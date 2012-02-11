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

/* local headers */

/* class variables */


typedef void (*mylua_callback_print) (void *handle, const char *string);


QMap<lua_State*,MyLua*> MyLua::cAllLua;


int MyLua::lua_cmd_print( lua_State *L )
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


MyLua::MyLua( QObject *parent )
: QThread( parent )
, mpL( lua_open() )
, mMutex()
{
   cAllLua.insert( mpL, this );
   luaL_openlibs( mpL );
   lua_pushstring( mpL, "print" );
   lua_pushcfunction( mpL, MyLua::lua_cmd_print );
   lua_rawset( mpL, LUA_GLOBALSINDEX );
   moveToThread( this );
}


MyLua::~MyLua()
{
   QMutexLocker locker(&mMutex);
   cAllLua.remove( mpL );
   lua_close( mpL );
}


void MyLua::run()
{
   exec();
}


void MyLua::runCode( const QString &data )
{
   QMutexLocker locker(&mMutex);
   int errorcode = luaL_loadstring( mpL, data.toUtf8().constData() );
   if( !errorcode )
   {
      errorcode = lua_pcall( mpL, 0, 0, 0 );
   }
   //locker.unlock();
   if( errorcode )
   {
      emit error( QString( lua_tostring( mpL, -1 ) ) );
      lua_pop( mpL, 1 );
   }
   else
   {
      emit success();
   }
}


void MyLua::setString( const QByteArray &name, const QString &value )
{
   QMutexLocker locker(&mMutex);
   lua_pushstring( mpL, value.toUtf8().constData() );
   lua_setglobal( mpL, name.constData() );
}


QString MyLua::getString( const QByteArray &name )
{
   QMutexLocker locker(&mMutex);
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
   QMutexLocker locker(&mMutex);
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
   QMutexLocker locker(&mMutex);
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