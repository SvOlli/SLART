/*
 * src/libs/Common/MyLua.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

/* system headers */
extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

/* class declaration */
#include "MyLua.hpp"

/* Qt headers */
#include <QtEndian>
#include <QStringList>

/* local library headers */
#include <Settings.hpp>

/* local headers */

/* class variables */
QMap<lua_State*,MyLua*> MyLua::cAllLua;


int MyLua::luaCmdPrint( lua_State *L )
{
   QStringList output;

   MyLua *instance = MyLua::cAllLua.value( L, 0 );
   if( !instance )
   {
      return 0;
   }

   int i = 0;
   for (i = 1; i <= lua_gettop(L); i++)
   {
      switch( lua_type( L, i ) )
      {
      case LUA_TNONE:
         output.append( "(none)" );
         break;
      case LUA_TNIL:
         output.append( "(nil)" );
         break;
      case LUA_TBOOLEAN:
         output.append( lua_toboolean(L,i) ? "(true)" : "(false)" );
         break;
      case LUA_TSTRING:
         output.append( QString::fromUtf8( lua_tostring(L,i) ) );
         break;
      case LUA_TTABLE:
         output.append( "(not table support yet)" );
         break;
      default:
         /* later */
         break;
      }
   }
   emit instance->print( output.join("\n") );
   return 0;
}


int MyLua::luaCmdStringCamel( lua_State *L )
{
   size_t l;
   int i;
   const char *s = luaL_checklstring(L, 1, &l);

   QString string( QString::fromUtf8(s) );
   bool nextUpper = true;
   for( i = 0; i < string.size(); i++ )
   {
      if( nextUpper )
      {
         string.replace( i, 1, string.at(i).toUpper() );
      }
      else
      {
         string.replace( i, 1, string.at(i).toLower() );
      }

      switch( string.at(i).toAscii() )
      {
         case ' ':
         case '.':
         case '-':
            nextUpper = true;
            break;
         case '(':
         case ')':
         case '[':
         case ']':
         case '\'':
         case '"':
            break;
         default:
            nextUpper = false;
            break;
      }
   }

   lua_pushstring( L, string.toUtf8().constData() );
   return 1;
}


MyLua::MyLua( QObject *parent )
: QThread( parent )
, mpL( lua_open() )
, mDataMutex()
, mpCallerMutex( new QMutex() )
{
   cAllLua.insert( mpL, this );
   luaL_openlibs( mpL );

   /* add "print" function */
   lua_pushstring( mpL, "print" );
   lua_pushcfunction( mpL, MyLua::luaCmdPrint );
   lua_rawset( mpL, LUA_GLOBALSINDEX );

   /* add string.camel function */
   /* get "string" table */
   lua_pushstring( mpL, "string" );
   lua_gettable( mpL, LUA_GLOBALSINDEX );
   /* add "camel" function */
   lua_pushstring( mpL, "camel" );
   lua_pushcfunction( mpL, MyLua::luaCmdStringCamel );
   lua_settable( mpL, -3 );
   /* write back "string" table */
   lua_setglobal( mpL, "string" );

   moveToThread( this );
}


MyLua::~MyLua()
{
   QMutexLocker locker(&mDataMutex);
   cAllLua.remove( mpL );
   lua_close( mpL );
   delete mpCallerMutex;
}


QMutex* MyLua::mutex( bool locked )
{
   if( locked )
   {
      mpCallerMutex->lock();
   }
   return mpCallerMutex;
}


void MyLua::run()
{
   exec();
}


void MyLua::runCode( const QString &data, QObject *target,
                     const QString &successMethod, const QString &failMethod )
{
   QMutexLocker locker(&mDataMutex);
   int errorcode = luaL_loadstring( mpL, data.toUtf8().constData() );
   if( !errorcode )
   {
      errorcode = lua_pcall( mpL, 0, 0, 0 );
   }
   if( errorcode )
   {
      const QString errmsg( lua_tostring( mpL, -1 ) );
      if( !QMetaObject::invokeMethod( target, failMethod.toAscii().constData(), Qt::QueuedConnection,
                                      Q_ARG( QString, errmsg ) ) )
      {
         qFatal( "%s:%d call failed in %s", __FILE__, __LINE__, Q_FUNC_INFO );
      }
      lua_pop( mpL, 1 );
   }
   else
   {

      if( !QMetaObject::invokeMethod( target, successMethod.toAscii().constData(), Qt::QueuedConnection ) )
      {
         qFatal( "%s:%d call failed in %s", __FILE__, __LINE__, Q_FUNC_INFO );
      }
   }
}


void MyLua::setString( const QByteArray &name, const QString &value )
{
   QMutexLocker locker(&mDataMutex);
   lua_pushstring( mpL, value.toUtf8().constData() );
   lua_setglobal( mpL, name.constData() );
}


QString MyLua::getString( const QByteArray &name )
{
   QMutexLocker locker(&mDataMutex);
   QString retval;
   lua_getglobal( mpL, name.constData() );
   if( lua_isstring( mpL, -1 ) )
   {
      retval = QString::fromUtf8( lua_tostring( mpL, -1 ) );
   }
   lua_pop( mpL, 1 );
   return retval;
}


void MyLua::setTable( const QByteArray &name, const MyLuaTable &values )
{
   QMutexLocker locker(&mDataMutex);
   lua_newtable( mpL );
   foreach( const QString &key, values.keys() )
   {
      const QString &value( values.value( key ) );
      lua_pushstring( mpL, key.toUtf8().constData() );
      lua_pushstring( mpL, value.toUtf8().constData() );
      lua_settable( mpL, -3 );
   }
   lua_setglobal( mpL, name.constData() );
}


MyLuaTable MyLua::getTable( const QByteArray &name )
{
   QMutexLocker locker(&mDataMutex);
   MyLuaTable table;

   lua_pushstring( mpL, name.constData() );
   lua_gettable( mpL, LUA_GLOBALSINDEX );
   lua_pushnil( mpL );
   while (lua_next( mpL, -2 ) != 0)
   {
      table.insert( QString( lua_tostring( mpL, -2 ) ), QString( lua_tostring( mpL, -1 ) ) );
      lua_pop( mpL, 1 );
   }
   lua_pop( mpL, 1 );
   return table;
}


int MyLua::getStack( QStringList *stack )
{
   int top = lua_gettop( mpL );

   if( stack )
   {
      stack->clear();
      for( int i = 1; i <= top; i++ )
      {  /* repeat for each level */
         switch( int t = lua_type( mpL, i ) )
         {
         case LUA_TSTRING:  /* strings */
            stack->append( QString("\"%1\"").arg( lua_tostring( mpL, i ) ) );
            break;
         case LUA_TBOOLEAN:  /* booleans */
            stack->append( QString(lua_toboolean( mpL, i ) ? "true" : "false") );
            break;
         case LUA_TNUMBER:  /* numbers */
            stack->append( QString("%1").arg( lua_tonumber( mpL, i ) ) );
            break;
         default:  /* other values */
            stack->append( QString("%1").arg( lua_typename( mpL, t ) ) );
            break;
         }
      }
   }
   return top;
}


QStringList MyLua::scriptNames( const QString &type )
{
   QSettings *settings = Settings::get("Lua");
   QString start( type );
   start.append("/");
   QStringList namesList;
   foreach( const QString &name, settings->allKeys() )
   {
      if( name.startsWith( start ) )
      {
         namesList << name.mid( start.size() ) ;
      }
   }
   return namesList;
}


void MyLua::setScript( const QString &type, const QString &name, const QString &script )
{
   QSettings *settings = Settings::get("Lua");
   QString key( QString("%1/%2").arg( type, name ) );
   if( script.isEmpty() )
   {
      settings->remove( key );
   }
   else
   {
      settings->setValue( key, script );
   }
}


QString MyLua::script( const QString &type, const QString &name )
{
   QSettings *settings = Settings::get("Lua");
   QString key( QString("%1/%2").arg( type, name ) );
   return settings->value( key ).toString();
}


QStringList MyLua::tableToStringList( const MyLuaTable &table )
{
   QStringList list;
   foreach( const QString &key, table.keys() )
   {
      list << QString("%1=%2").arg( key, table.value( key ) );
   }
   return list;
}


MyLuaTable MyLua::tableFromStringList( const QStringList &list )
{
   MyLuaTable table;
   foreach( const QString &entry, list )
   {
      int pos = entry.indexOf('=');
      table.insert( entry.left( pos ), entry.mid( pos + 1 ) );
   }
   return table;
}
