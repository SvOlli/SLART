/*
 * src/libs/Common/MyLua.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef MYLUA_HPP
#define MYLUA_HPP MYLUA_HPP

/* base class */
#include <QThread>

/* system headers */

/* Qt headers */
#include <QMap>
#include <QMutex>
#include <QStringList>

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */

/* forward declaration of local classes */


/*!
  \addtogroup Common
 @{
 */
#ifndef LUA_VERSION
typedef void lua_State; /* trick for preventing inclusion of lua.h */
#endif
typedef QMap<QString,QString> MyLuaTable;
/*!
 \brief simple wrapper for running lua scripts

 typical usage:
 \code
   class LuaDemo: public QObject
   {
   public:
      LuaDemo( QObject *parent = 0 )
      : QObject( parent )
      , mpLua( new MyLua( this ) )
      {
      }

      void runLua()
      {
         mpLua->mutex( true );
         // prepare data, etc.
         emit runCode( mpEditor->toPlainText(), this, "runSucceeded", "runFailed" );
      }

   public slots:
      void runSucceeded()
      {
         // handle data
         disconnect( mpLua, SIGNAL(print(QString)),
                     this, SLOT(log(QString)) );
         mpLua->mutex()->unlock();
      }

      void runFailed( const QString &msg )
      {
         // handle error
         disconnect( mpLua, SIGNAL(print(QString)),
                     this, SLOT(log(QString)) );
         mpLua->mutex()->unlock();
      }

   private:
      MyLua    *mpLua;
   }
 \endcode

*/
class MyLua : public QThread
{
Q_OBJECT

public:
   /*!
    \brief constructor

    \param parent parent object
   */
   MyLua( QObject *parent = 0 );

   /*!
    \brief destructor

   */
   virtual ~MyLua();

   /*!
    \brief set a string variable in the lua instance

    \param name name of variable
    \param value value of variable
   */
   void setString( const QByteArray &name, const QString &value );

   /*!
    \brief get a string variable in the lua instance

    \param name name of variable
    \return value value of variable
   */
   QString getString( const QByteArray &name );

   /*!
    \brief set a table variable in the lua instance

    \param name name of variable
    \param values value of variable
   */
   void setTable( const QByteArray &name, const MyLuaTable &values );

   /*!
    \brief set a table variable in the lua instance

    \param name name of variable
    \return values of table
   */
   MyLuaTable getTable( const QByteArray &name );

   /*!
    \brief get elements from stack for debugging purposes

    \param stack stringlist to write stack debug data into
    \return size of stack
   */
   int getStack( QStringList *stack );

   /*!
    \brief mutex used for exclusive access to lua instance

    \param locked lock mutex before returning
    \return QMutex* pointer of mutex to use
   */
   QMutex *mutex( bool locked = false );

   /*!
    \brief get the names of all scripts of a type

    \param type script type
    \return QStringList
   */
   static QStringList scriptNames( const QString &type );

   /*!
    \brief set a script

    \param type script type
    \param name script name
    \param script script lua code
   */
   static void setScript( const QString &type, const QString &name, const QString &script );

   /*!
    \brief get a script

    \param type script type
    \param name script name
    \return QString script lua code
   */
   static QString script( const QString &type, const QString &name );

   /*!
    \brief convert a table for lua to a string list in format of "key=value"

    \param table data to convert
    \return QStringList converted data
   */
   static QStringList tableToStringList( const MyLuaTable &table );

   /*!
    \brief convert a string list in format of "key=value" to a table for lua

    \param list data to convert
    \return MyLuaTable converted data
   */
   static MyLuaTable tableFromStringList( const QStringList &list );

public slots:
   /*!
    \brief slot for running code fragment

    \param data code to run
    \param target object to call after execution
    \param successCallback method to call on success
    \param failCallback method to call on failure
   */
   void runCode( const QString &data, QObject *target,
                 const QString &successCallback, const QString &failCallback );

signals:
   /*!
    \brief execution of code segment from runCode was successful

   */
   void success();

   /*!
    \brief compilation or execution of code segment from runCode was not successful

    \param data error message
   */
   void error( const QString &data );

   /*!
    \brief send data emitted by the script using the print command

    \param data data received from print
   */
   void print( const QString &data );

protected:
   /*!
    \brief main loop for handling runCode

    */
   void run();

private:
   Q_DISABLE_COPY( MyLua )

   /*!
    \brief internal use only: callback for lua command "print"

    \param L lua state
    \return int number of return values
   */
   static int luaCmdPrint( lua_State *L );

   /*!
    \brief internal use only: callback for lua command "string.camel"

    \param L lua state
    \return int number of return values
   */
   static int luaCmdStringCamel( lua_State *L );

   static QMap<lua_State*,MyLua*>   cAllLua;        /*!< \brief needed for finding *this from lua_State *L */
   lua_State                        *mpL;           /*!< \brief lua_State parameter for lua commands */
   QMutex                           mDataMutex;     /*!< \brief internal mutex for preventing early data access */
   QMutex                           *mpCallerMutex; /*!< \brief external mutex for providing exclusive access */
};

/*! @} */

#endif
