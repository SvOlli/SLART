/*
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
#include <QMessageBox>
#include <QtSql>

/* local library headers */
#include <TrackInfo.hpp>

/* local headers */

#include "Trace.hpp"


DatabaseInterface *DatabaseInterface::cpInterface = 0;


DatabaseInterface *DatabaseInterface::create()
{
   if( !cpInterface )
   {
      cpInterface = new DatabaseInterface();
   }
   return cpInterface;
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


DatabaseInterface::DatabaseInterface( const QString &fileName )
: mpThread( new QThread() )
, mpDatabase( new DatabaseThread( fileName ) )
{
   mpDatabase->moveToThread( mpThread );
   mpThread->connect( qApp, SIGNAL(aboutToQuit()),
                      mpThread, SLOT(quit()) );
   mpThread->start();
}


DatabaseInterface::~DatabaseInterface()
{
   mpDatabase->call( mpThread, SLOT(quit()) );
   delete mpDatabase;
   delete mpThread;
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


void DatabaseInterface::registerUpdate( QObject *satellite, const QByteArray &message )
{
   mpDatabase->registerUpdate( satellite, message );
}


void DatabaseInterface::disableNotify()
{
   QMetaObject::invokeMethod( mpDatabase, "disableNotify",
                              Qt::QueuedConnection );
}


void DatabaseInterface::getTrackInfo( QObject *target, const char *method,
                                      int id,
                                      const QVariant &payload )
{
   QMetaObject::invokeMethod( mpDatabase, "getTrackInfo",
                              Qt::QueuedConnection,
                              Q_ARG( QObject*, target ),
                              Q_ARG( QByteArray, QMetaObject::normalizedSignature( method ) ),
                              Q_ARG( int, id ),
                              Q_ARG( QString, QString() ),
                              Q_ARG( QVariant, payload ) );
}


void DatabaseInterface::getTrackInfo( QObject *target, const char *method,
                                      const QString &fileName,
                                      const QVariant &payload )
{
   QMetaObject::invokeMethod( mpDatabase, "getTrackInfo",
                              Qt::QueuedConnection,
                              Q_ARG( QObject*, target ),
                              Q_ARG( QByteArray, QMetaObject::normalizedSignature( method ) ),
                              Q_ARG( int, 0 ),
                              Q_ARG( QString, fileName ),
                              Q_ARG( QVariant, payload ) );
}


void DatabaseInterface::getTrackInfoList( QObject *target, const char *method,
                                          const QString &search,
                                          const QVariant &payload )
{
   QMetaObject::invokeMethod( mpDatabase, "getTrackInfoList",
                              Qt::QueuedConnection,
                              Q_ARG( QObject*, target ),
                              Q_ARG( QByteArray, QMetaObject::normalizedSignature( method ) ),
                              Q_ARG( QString, search ),
                              Q_ARG( QVariant, payload ) );
}


void DatabaseInterface::getPathNameList( QObject *target, const char *method,
                                         const QString &search,
                                         const QVariant &payload )
{
   QMetaObject::invokeMethod( mpDatabase, "getPathNameList",
                              Qt::QueuedConnection,
                              Q_ARG( QObject*, target ),
                              Q_ARG( QByteArray, QMetaObject::normalizedSignature( method ) ),
                              Q_ARG( QString, search ),
                              Q_ARG( QVariant, payload ) );
}


void DatabaseInterface::getRandomTrack( QObject *target, const char *method,
                                        bool favorite, bool leastplayed,
                                        const QStringList &excludeArtists,
                                        const QString &folder,
                                        const QVariant &payload )
{
   QMetaObject::invokeMethod( mpDatabase, "getRandomTrack",
                              Qt::QueuedConnection,
                              Q_ARG( QObject*, target ),
                              Q_ARG( QByteArray, QMetaObject::normalizedSignature( method ) ),
                              Q_ARG( bool, favorite ),
                              Q_ARG( bool, leastplayed ),
                              Q_ARG( QStringList, excludeArtists ),
                              Q_ARG( QString, folder ),
                              Q_ARG( QVariant, payload ) );
}


void DatabaseInterface::getGroups( QObject *target, const char *method,
                                    const QVariant &payload )
{
   QMetaObject::invokeMethod( mpDatabase, "getGroups",
                              Qt::QueuedConnection,
                              Q_ARG( QObject*, target ),
                              Q_ARG( QByteArray, QMetaObject::normalizedSignature( method ) ),
                              Q_ARG( QVariant, payload ) );
}


void DatabaseInterface::getGroup( QObject *target, const char *method,
                                   const QString &folder,
                                   const QVariant &payload )
{
   QMetaObject::invokeMethod( mpDatabase, "getGroup",
                              Qt::QueuedConnection,
                              Q_ARG( QObject*, target ),
                              Q_ARG( QByteArray, QMetaObject::normalizedSignature( method ) ),
                              Q_ARG( QString, folder ),
                              Q_ARG( QVariant, payload ) );
}


void DatabaseInterface::updateTrackInfo( const TrackInfo &trackInfo, bool allowinsert )
{
   QMetaObject::invokeMethod( mpDatabase, "updateTrackInfo",
                              Qt::QueuedConnection,
                              Q_ARG( TrackInfo, trackInfo ),
                              Q_ARG( bool, allowinsert ) );
}


void DatabaseInterface::deleteTrackInfo( const TrackInfo &trackInfo )
{
   QMetaObject::invokeMethod( mpDatabase, "deleteTrackInfo",
                              Qt::QueuedConnection,
                              Q_ARG( TrackInfo, trackInfo ) );
}


void DatabaseInterface::deleteTrackInfo( const QString &fileName )
{
   QMetaObject::invokeMethod( mpDatabase, "getTrackInfo",
                              Qt::QueuedConnection,
                              Q_ARG( QObject*, mpDatabase ),
                              Q_ARG( QString, "deleteTrackInfo" ),
                              Q_ARG( int, 0 ),
                              Q_ARG( QString, fileName ) );
}


void DatabaseInterface::insertGroup( const QString &folder )
{
   QMetaObject::invokeMethod( mpDatabase, "insertGroup",
                              Qt::QueuedConnection,
                              Q_ARG( QString, folder ) );
}


void DatabaseInterface::deleteGroup( const QString &folder )
{
   QMetaObject::invokeMethod( mpDatabase, "deleteGroup",
                              Qt::QueuedConnection,
                              Q_ARG( QString, folder ) );
}


void DatabaseInterface::rename( const QString &oldName, const QString &newName )
{
   QMetaObject::invokeMethod( mpDatabase, "rename",
                              Qt::QueuedConnection,
                              Q_ARG( QString, oldName ),
                              Q_ARG( QString, newName ) );
}

void DatabaseInterface::getAllColumnData( QObject *target, const char *method,
                                          Column column,
                                          const QVariant &payload )
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
      qFatal( "%s:%d: illegal enum value in %s", __FILE__, __LINE__, Q_FUNC_INFO );
   }

   QMetaObject::invokeMethod( mpDatabase, "getAllColumnData",
                              Qt::QueuedConnection,
                              Q_ARG( QObject*, target ),
                              Q_ARG( QByteArray, QMetaObject::normalizedSignature( method ) ),
                              Q_ARG( QString, columnName ),
                              Q_ARG( QVariant, payload ) );
}


void DatabaseInterface::generateTestLoad( QObject *target, const char *method,
                                          const QString &command, const QVariant &payload )
{
   QMetaObject::invokeMethod( mpDatabase, "generateTestLoad",
                              Qt::QueuedConnection,
                              Q_ARG( QObject*, target ),
                              Q_ARG( QByteArray, QMetaObject::normalizedSignature( method ) ),
                              Q_ARG( QString, command ),
                              Q_ARG( QVariant, payload ) );
}


void DatabaseInterface::call( QObject *target, const char *method,
                              const QVariant &payload )
{
   QMetaObject::invokeMethod( mpDatabase, "call",
                              Qt::QueuedConnection,
                              Q_ARG( QObject*, target ),
                              Q_ARG( QByteArray, QMetaObject::normalizedSignature( method ) ),
                              Q_ARG( QVariant, payload ) );
}
