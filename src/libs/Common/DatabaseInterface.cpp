/**
 * src/libs/Common/DatabaseInterface.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

/* class declaration */
#include "DatabaseInterface.hpp"

/* system headers */
#include <time.h>

/* Qt headers */
#include <QApplication>
#include <QDir>
#include <QFileInfo>
#include <QMessageBox>
#include <QtSql>

/* local library headers */

/* local headers */
#include "DatabaseThread.hpp"
#include "MySettings.hpp"
#include "Satellite.hpp"

#include "Trace.hpp"


DatabaseInterface *DatabaseInterface::cpInterface = 0;


void DatabaseInterface::create()
{
   if( !cpInterface )
   {
      cpInterface = new DatabaseInterface();
   }
}


DatabaseInterface *DatabaseInterface::get()
{
   return cpInterface;
}


void DatabaseInterface::destroy()
{
   if( cpInterface )
   {
      delete cpInterface;
      cpInterface = 0;
   }
}


bool DatabaseInterface::exists()
{
   return QFileInfo( DatabaseThread::getDatabaseFileName() ).isFile();
}


DatabaseInterface::DatabaseInterface( const QString &fileName )
: mpDatabase( new DatabaseThread( fileName ) )
{
}


DatabaseInterface::~DatabaseInterface()
{
   call( mpDatabase, "quit" );
   mpDatabase->wait();
   delete mpDatabase;
}


void DatabaseInterface::connectActivityIndicator( QObject *object, const char *slot )
{
   mpDatabase->connect( mpDatabase, SIGNAL(working(bool)),
                        object, slot );
}


void DatabaseInterface::disconnectActivityIndicator( QObject *object, const char *slot )
{
   mpDatabase->disconnect( mpDatabase, SIGNAL(working(bool)),
                           object, slot );
}


void DatabaseInterface::registerUpdate( Satellite *satellite, const QByteArray &message )
{
   mpDatabase->registerUpdate( satellite, message );
}


void DatabaseInterface::disableNotify()
{
   QMetaObject::invokeMethod( mpDatabase, "disableNotify",
                              Qt::QueuedConnection );
}


void DatabaseInterface::getTrackInfo( QObject *target, const QString &method,
                                      int id )
{
   QMetaObject::invokeMethod( mpDatabase, "getTrackInfo",
                              Qt::QueuedConnection,
                              Q_ARG( QObject*, target ),
                              Q_ARG( const QString&, method ),
                              Q_ARG( int, id ),
                              Q_ARG( const QString&, QString() ) );
}


void DatabaseInterface::getTrackInfo( QObject *target, const QString &method,
                                      const QString &fileName )
{
   QMetaObject::invokeMethod( mpDatabase, "getTrackInfo",
                              Qt::QueuedConnection,
                              Q_ARG( QObject*, target ),
                              Q_ARG( const QString&, method ),
                              Q_ARG( int, 0 ),
                              Q_ARG( const QString&, fileName ) );
}


void DatabaseInterface::getTrackInfoList( QObject *target, const QString &method,
                                          const QString &search)
{
   QMetaObject::invokeMethod( mpDatabase, "getTrackInfoList",
                              Qt::QueuedConnection,
                              Q_ARG( QObject*, target ),
                              Q_ARG( const QString&, method ),
                              Q_ARG( const QString&, search ) );
}


void DatabaseInterface::getPathNameList( QObject *target, const QString &method,
                                         const QString &search )
{
   QMetaObject::invokeMethod( mpDatabase, "getPathNameList",
                              Qt::QueuedConnection,
                              Q_ARG( QObject*, target ),
                              Q_ARG( const QString&, method ),
                              Q_ARG( const QString&, search ) );
}


void DatabaseInterface::getRandomTrack( QObject *target, const QString &method,
                                        bool favorite, bool leastplayed,
                                        const QString &folder )
{
   QMetaObject::invokeMethod( mpDatabase, "getRandomTrack",
                              Qt::QueuedConnection,
                              Q_ARG( QObject*, target ),
                              Q_ARG( const QString&, method ),
                              Q_ARG( bool, favorite ),
                              Q_ARG( bool, leastplayed ),
                              Q_ARG( const QString&, folder ) );
}


void DatabaseInterface::getFolders( QObject *target, const QString &method )
{
   QMetaObject::invokeMethod( mpDatabase, "getFolders",
                              Qt::QueuedConnection,
                              Q_ARG( QObject*, target ),
                              Q_ARG( const QString&, method ) );
}


void DatabaseInterface::getFolder( QObject *target, const QString &method,
                                   const QString &folder )
{
   QMetaObject::invokeMethod( mpDatabase, "getFolder",
                              Qt::QueuedConnection,
                              Q_ARG( QObject*, target ),
                              Q_ARG( const QString&, method ),
                              Q_ARG( const QString&, folder ) );
}


void DatabaseInterface::updateTrackInfo( const TrackInfo &trackInfo, bool allowinsert )
{
   QMetaObject::invokeMethod( mpDatabase, "updateTrackInfo",
                              Qt::QueuedConnection,
                              Q_ARG( const TrackInfo&, trackInfo ),
                              Q_ARG( bool, allowinsert ) );
}


void DatabaseInterface::deleteTrackInfo( const TrackInfo &trackInfo )
{
   QMetaObject::invokeMethod( mpDatabase, "deleteTrackInfo",
                              Qt::QueuedConnection,
                              Q_ARG( const TrackInfo&, trackInfo ) );
}


void DatabaseInterface::deleteTrackInfo( const QString &fileName )
{
   QMetaObject::invokeMethod( mpDatabase, "getTrackInfo",
                              Qt::QueuedConnection,
                              Q_ARG( QObject*, mpDatabase ),
                              Q_ARG( const QString&, "deleteTrackInfo" ),
                              Q_ARG( int, 0 ),
                              Q_ARG( const QString&, fileName ) );
}


void DatabaseInterface::insertFolder( const QString &folder )
{
   QMetaObject::invokeMethod( mpDatabase, "insertFolder",
                              Qt::QueuedConnection,
                              Q_ARG( const QString&, folder ) );
}


void DatabaseInterface::deleteFolder( const QString &folder )
{
   QMetaObject::invokeMethod( mpDatabase, "deleteFolder",
                              Qt::QueuedConnection,
                              Q_ARG( const QString&, folder ) );
}


void DatabaseInterface::rename( const QString &oldName, const QString &newName )
{
   QMetaObject::invokeMethod( mpDatabase, "rename",
                              Qt::QueuedConnection,
                              Q_ARG( const QString&, oldName ),
                              Q_ARG( const QString&, newName ) );
}

void DatabaseInterface::getAllColumnData( QObject *target, const QString &method,
                                          Column column )
{
   QString columnName;
   switch( column )
   {
   case Artist:
      columnName = "Artist";
      break;
   case Title:
      columnName = "Title";
      break;
   case Album:
      columnName = "Album";
      break;
   case Genre:
      columnName = "Genre";
      break;
   default:
      qFatal( "%s: illegal enum value", Q_FUNC_INFO );
   }

   QMetaObject::invokeMethod( mpDatabase, "getAllColumnData",
                              Qt::QueuedConnection,
                              Q_ARG( QObject*, target ),
                              Q_ARG( const QString&, method ),
                              Q_ARG( const QString&, columnName ) );
}


void DatabaseInterface::call( QObject *target, const QString &method )
{
   QMetaObject::invokeMethod( mpDatabase, "call",
                              Qt::QueuedConnection,
                              Q_ARG( QObject*, target ),
                              Q_ARG( const QString&, method ) );
}
