/**
 * src/libs/Common/DatabaseThread.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

/* class declaration */
#include "DatabaseThread.hpp"

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
#include "MySettings.hpp"
#include "Satellite.hpp"

#ifndef SQLITE_BUSY
#define SQLITE_BUSY    5
#define SQLITE_LOCKED  6
#endif

#include "Trace.hpp"


QString DatabaseThread::getDatabaseFileName()
{
#ifdef Q_OS_WIN32
   QString slartdb( "/slart.db" );
#else
   QString slartdb( "/.slartdb" );
#endif
   return QDir::homePath() + slartdb;
}


DatabaseThread::DatabaseThread( const QString &fileName )
: QThread()
, mpSqlDB( 0 )
, mpQuery( 0 )
, mpCommitTimer( 0 )
, mDatabaseVersion( 0 )
, mCodeVersion( 1 )
{
   qsrand( time((time_t*)0) );
   mpSqlDB = new QSqlDatabase( QSqlDatabase::addDatabase( "QSQLITE" ) );
   mpCommitTimer = new QTimer( this );
   mpCommitTimer->setSingleShot( true );
   mpCommitTimer->setInterval( 250 );
   connect( mpCommitTimer, SIGNAL(timeout()),
            this, SLOT(commit()) );

   qRegisterMetaType<TrackInfo>( "TrackInfo" );
   qRegisterMetaType<TrackInfoList>( "TrackInfoList" );

   if( mpSqlDB->lastError().type() != QSqlError::NoError )
   {
      QMessageBox::critical( 0, QApplication::applicationName() + QWidget::tr(": Error"),
                             QWidget::tr("Could not open database.\nPlease make sure that the SQLite driver for Qt is installed.") );
      exit(1);
   }

   if( fileName.isEmpty() )
   {
      mpSqlDB->setDatabaseName( getDatabaseFileName() );
   }
   else
   {
      mpSqlDB->setDatabaseName( fileName );
   }

   if(!mpSqlDB->open())
   {
      // TODO: some error handling
      logError( QString("open() failed\nDatabase: ") + mpSqlDB->lastError().driverText() );
   }
   mpQuery = new QSqlQuery;

   if( mpQuery->exec( "SELECT value FROM slart_config WHERE key = 'Version';" ) )
   {
      if( mpQuery->next() )
      {
         mDatabaseVersion = mpQuery->value(0).toUInt();
      }
   }
   else
   {
      logError();
   }
   mpQuery->clear();

   if( !mDatabaseVersion )
   {
      /* create database */
      QStringList initSQL;
      initSQL
            << "CREATE TABLE slart_config (key VARCHAR PRIMARY KEY,"
            "value VARCHAR);"
            << "INSERT OR REPLACE INTO slart_config(key,value) VALUES ('Version'," +
            QString::number(mCodeVersion) + ");"

            << "CREATE TABLE slart_tracks (id INTEGER PRIMARY KEY,"
            "Directory VARCHAR,"
            "FileName VARCHAR,"
            "Artist VARCHAR,"
            "Title VARCHAR,"
            "Album VARCHAR,"
            "TrackNr INTEGER,"
            "Year INTEGER,"
            "Genre VARCHAR,"
            "PlayTime INTEGER,"
            "LastScanned INTEGER,"
            "LastTagsRead INTEGER,"
            "TimesPlayed INTEGER,"
            "Volume DOUBLE,"
            "Folders VARCHAR,"
            "Flags INTEGER);"
            << "CREATE UNIQUE INDEX slart_tracks_file ON slart_tracks (Directory,FileName);"
            << "CREATE INDEX slart_tracks_filename ON slart_tracks (FileName);"
            << "CREATE INDEX slart_tracks_artist ON slart_tracks (Artist);"
            << "CREATE INDEX slart_tracks_title ON slart_tracks (Title);"

            << "CREATE TABLE slart_folders (id INTEGER PRIMARY KEY,"
            "Name VARCHAR,"
            "FileName VARCHAR);"
            << "CREATE UNIQUE INDEX slart_folders_name ON slart_folders (Name);"

            ;

      foreach( const QString &statement, initSQL )
      {
         if( !mpQuery->exec( statement ) )
         {
            logError();
         }
         mpQuery->clear();
      }
   }
   else
   {
      if( mDatabaseVersion < mCodeVersion )
      {
         QStringList updateSQL;
         /* upgrade database */
         switch( mDatabaseVersion )
         {
         case 1:
            updateSQL
                  << ""
                  ;
            // slip through
         case 2:
            break;
         default:
            // should not happen
            break;
         }

         updateSQL
               << "UPDATE TABLE slart_config SET value = " + QString::number( mCodeVersion )
               + " WHERE key = 'Version';"
               ;
         foreach( const QString &statement, updateSQL )
         {
            if( !mpQuery->exec( statement ) )
            {
               logError();
            }
            mpQuery->clear();
         }
      }
   }
   mpQuery->clear();
   moveToThread( this );
   start();
}


DatabaseThread::~DatabaseThread()
{
   if( isRunning() )
   {
      quit();
      wait();
   }
   if( mpQuery )
   {
      mpQuery->clear();
      delete mpQuery;
   }
   if( mpSqlDB )
   {
      mpSqlDB->commit();
      mpSqlDB->close();
      /* found on: http://lists.trolltech.com/qt4-preview-feedback/2005-08/thread00142-0.html#msg00143 */
      *mpSqlDB = QSqlDatabase();
      delete mpSqlDB;
   }
}


void DatabaseThread::run()
{
   exec();
}


void DatabaseThread::prepare()
{
   if( mUpdateCount > 100 )
   {
      commit();
   }
   if( !mUpdateCount )
   {
      mpSqlDB->transaction();
      emit working( true );
   }
   mpCommitTimer->start();
}


void DatabaseThread::commit()
{
   mpSqlDB->commit();
   mUpdateCount = 0;
   emit working( false );
}


void DatabaseThread::getTrackInfo( QObject *target, const QString &method,
                                   int id, const QString &fileName )
{
   TrackInfo trackInfo;

   QString sql( "SELECT id,Directory,FileName,Artist,Title,Album,"
                "CASE WHEN TrackNr NOTNULL THEN TrackNr ELSE -1 END,"
                "CASE WHEN Year NOTNULL THEN Year ELSE -1 END,"
                "Genre,PlayTime,LastScanned,LastTagsRead,TimesPlayed,Volume,Folders,Flags"
                " FROM slart_tracks WHERE ");

   if( fileName.isEmpty() )
   {
      sql.append( "id = :id ;" );
      mpQuery->prepare( sql );
      mpQuery->bindValue( ":id", id );
   }
   else
   {
      int fileNameStart = fileName.lastIndexOf('/');
      sql.append( "Directory = :directory AND FileName = :fileName ;" );

      mpQuery->prepare( sql );
      mpQuery->bindValue( ":directory", fileName.left(fileNameStart) );
      mpQuery->bindValue( ":fileName", fileName.mid(fileNameStart+1) );
   }
   emit working( true );
   if( !mpQuery->exec() )
   {
      logError();
   }

   if( mpQuery->next() )
   {
      trackInfo.mID           = mpQuery->value( 0).toUInt();
      trackInfo.mDirectory    = mpQuery->value( 1).toString();
      trackInfo.mFileName     = mpQuery->value( 2).toString();
      trackInfo.mArtist       = mpQuery->value( 3).toString();
      trackInfo.mTitle        = mpQuery->value( 4).toString();
      trackInfo.mAlbum        = mpQuery->value( 5).toString();
      trackInfo.mTrackNr      = mpQuery->value( 6).toInt();
      trackInfo.mYear         = mpQuery->value( 7).toInt();
      trackInfo.mGenre        = mpQuery->value( 8).toString();
      trackInfo.mPlayTime     = mpQuery->value( 9).toUInt();
      trackInfo.mLastScanned  = mpQuery->value(10).toUInt();
      trackInfo.mLastTagsRead = mpQuery->value(11).toUInt();
      trackInfo.mTimesPlayed  = mpQuery->value(12).toUInt();
      trackInfo.mVolume       = mpQuery->value(13).toDouble();
      trackInfo.mFolders      = mpQuery->value(14).toString();
      trackInfo.mFlags        = mpQuery->value(15).toUInt();

      mpQuery->clear();
   }
   else
   {
      trackInfo.mID = 0;
      trackInfo.mArtist.clear();
      trackInfo.mTitle.clear();
      trackInfo.mAlbum.clear();
      trackInfo.mTrackNr = -1;
      trackInfo.mYear = -1;
      trackInfo.mGenre.clear();
      trackInfo.mTimesPlayed = 0;
      trackInfo.mVolume = 0.0;
      trackInfo.mFolders.clear();
      trackInfo.mFlags = 0;

      mpQuery->clear();
   }
   emit working( false );

   if( !QMetaObject::invokeMethod( target, method.toAscii().constData(), Qt::QueuedConnection,
                                   Q_ARG( TrackInfo, trackInfo ) ) )
   {
      qFatal( "%s:%d call failed in %s", __FILE__, __LINE__, Q_FUNC_INFO );
   }
}


void DatabaseThread::getTrackInfoList( QObject *target, const QString &method,
                                       const QString &search )
{
   TrackInfoList trackInfoList;

   QString sqlSearch( "%" );
   if( !search.isEmpty() )
   {
      sqlSearch.append( search );
      sqlSearch.replace( "*", "%" );
      sqlSearch.replace( "?", "_" );
      sqlSearch.append( "%" );
   }
   mpQuery->clear();
   mpQuery->prepare( "SELECT Directory,FileName,Artist,Title,Album,"
                     "CASE WHEN TrackNr NOTNULL THEN TrackNr ELSE -1 END,"
                     "CASE WHEN Year NOTNULL THEN Year ELSE -1 END,"
                     "Genre,PlayTime,LastScanned,LastTagsRead,TimesPlayed,Volume,Folders,Flags,id"
                     " FROM slart_tracks WHERE Directory LIKE :directory OR FileName LIKE :fileName;" );
   mpQuery->bindValue( ":directory", sqlSearch );
   mpQuery->bindValue( ":fileName", sqlSearch );
   emit working( true );
   if( !mpQuery->exec() )
   {
      logError();
   }

   while( mpQuery->next() )
   {
      trackInfoList << TrackInfo( mpQuery->value( 0).toString(),
                                  mpQuery->value( 1).toString(),
                                  mpQuery->value( 2).toString(),
                                  mpQuery->value( 3).toString(),
                                  mpQuery->value( 4).toString(),
                                  mpQuery->value( 5).toInt(),
                                  mpQuery->value( 6).toInt(),
                                  mpQuery->value( 7).toString(),
                                  mpQuery->value( 8).toUInt(),
                                  mpQuery->value( 9).toUInt(),
                                  mpQuery->value(10).toUInt(),
                                  mpQuery->value(11).toUInt(),
                                  mpQuery->value(12).toDouble(),
                                  mpQuery->value(13).toString(),
                                  mpQuery->value(14).toUInt() );
   }
   mpQuery->clear();
   emit working( false );

   if( !QMetaObject::invokeMethod( target, method.toAscii().constData(), Qt::QueuedConnection,
                                   Q_ARG( TrackInfoList, trackInfoList ) ) )
   {
      qFatal( "%s:%d call failed in %s", __FILE__, __LINE__, Q_FUNC_INFO );
   }
}


void DatabaseThread::getPathNameList( QObject *target, const QString &method,
                                      const QString &search )
{
   QStringList pathNameList;

   QString sqlSearch( "%" );
   if( !search.isEmpty() )
   {
      sqlSearch.append( search );
      sqlSearch.replace( "*", "%" );
      sqlSearch.replace( "?", "_" );
      sqlSearch.append( "%" );
   }
   mpQuery->clear();
   mpQuery->prepare( "SELECT Directory,FileName"
                     " FROM slart_tracks WHERE Directory LIKE :directory OR FileName LIKE :fileName;" );
   mpQuery->bindValue( ":directory", sqlSearch );
   mpQuery->bindValue( ":fileName", sqlSearch );
   emit working( true );
   if( !mpQuery->exec() )
   {
      logError();
   }

   while( mpQuery->next() )
   {
      pathNameList << mpQuery->value( 0).toString()
              + "/" + mpQuery->value( 1).toString();
   }
   mpQuery->clear();
   emit working( false );

   if( !QMetaObject::invokeMethod( target, method.toAscii().constData(), Qt::QueuedConnection,
                                   Q_ARG( QStringList, pathNameList ) ) )
   {
      qFatal( "%s:%d call failed in %s", __FILE__, __LINE__, Q_FUNC_INFO );
   }
}


void DatabaseThread::getRandomTrack( QObject *target, const QString &method,
                                     bool favorite, bool leastplayed,
                                     const QString &folder )
{
   TrackInfo trackInfo;

   QString sql( "SELECT id FROM slart_tracks WHERE Flags & " );

   if( favorite )
   {
      sql.append( QString::number( (unsigned int)TrackInfo::Favorite ) );
   }
   else
   {
      sql.append( QString::number( (unsigned int)TrackInfo::Unwanted ) );
      sql.append( " = 0" );
   }

   if( leastplayed )
   {
      sql.append( " AND TimesPlayed = (SELECT MIN(TimesPlayed) FROM slart_tracks)" );
   }

   if( !folder.isEmpty() )
   {
      sql.append( " AND Folders LIKE '%|" );
      sql.append( folder );
      sql.append( "|%'" );
   }

   sql.append( ";" );

   mpQuery->prepare( sql );
   emit working( true );
   if( !mpQuery->exec() )
   {
      logError();
   }

   /* QSqlQuery::size() seems not to work with sqlite... :( */
   int rows;
   for( rows = 0; mpQuery->next(); rows++ )
      ;

   if( rows )
   {
      int row = qrand() % rows;
      mpQuery->seek( row );
      int id = mpQuery->value(0).toUInt();
      mpQuery->clear();
      emit working( false );
      getTrackInfo( target, method, id, QString() );
   }
   else
   {
      mpQuery->clear();
      emit working( false );
      bool success = false;
      if( !QMetaObject::invokeMethod( target, method.toAscii().constData(), Qt::QueuedConnection,
                                      Q_ARG( bool, success ),
                                      Q_ARG( TrackInfo, trackInfo ) ) )
      {
         qFatal( "%s:%d call failed in %s", __FILE__, __LINE__, Q_FUNC_INFO );
      }
   }
}


void DatabaseThread::getFolders( QObject *target, const QString &method )
{
   QStringList folders;

   mpQuery->prepare( "SELECT Name FROM slart_folders ORDER BY Name;" );
   emit working( true );
   if( !mpQuery->exec() )
   {
      logError();
   }
   while( mpQuery->next() )
   {
      folders << mpQuery->value(0).toString();
   }
   mpQuery->clear();
   emit working( false );

   if( !QMetaObject::invokeMethod( target, method.toAscii().constData(), Qt::QueuedConnection,
                                   Q_ARG( const QStringList &, folders ) ) )
   {
      qFatal( "%s:%d call failed in %s", __FILE__, __LINE__, Q_FUNC_INFO );
   }
}


void DatabaseThread::getFolder( QObject *target, const QString &method,
                                const QString &folder )
{
   QStringList folders;

   if( !folder.isEmpty() )
   {
      QString sql( "SELECT Directory, FileName FROM slart_tracks WHERE " );

      if( folder == QChar(1) )
      {
         sql.append( "Flags & " );
         sql.append( QString::number( (unsigned int)TrackInfo::Favorite ) );
      }
      else if( folder == QChar(2) )
      {
         sql.append( "Flags & " );
         sql.append( QString::number( (unsigned int)TrackInfo::Unwanted ) );
      }
      else
      {
         sql.append( "Folders LIKE '%|" );
         sql.append( folder );
         sql.append( "|%'" );
      }
      sql.append( " ORDER BY Directory, FileName;" );

      mpQuery->prepare( sql );
      emit working( true );
      if( !mpQuery->exec() )
      {
         logError();
      }
      while( mpQuery->next() )
      {
         folders << mpQuery->value(0).toString() + '/' + mpQuery->value(1).toString();
      }
      mpQuery->clear();
      emit working( false );
   }

   if( !QMetaObject::invokeMethod( target, method.toAscii().constData(), Qt::QueuedConnection,
                                   Q_ARG( QStringList, folders ) ) )
   {
      qFatal( "%s:%d call failed in %s", __FILE__, __LINE__, Q_FUNC_INFO );
   }
}


void DatabaseThread::logError( const QString &note )
{
   QString msg( "I0L\nApplication: " );
   msg.append( QApplication::applicationName() );
   if( !note.isEmpty() )
   {
      msg.append( "\nNote: " );
      msg.append( note );
   }
   msg.append( "\nDatabase: " );
   msg.append( mpSqlDB->lastError().databaseText() );
   if( mpQuery )
   {
      msg.append( "\nDriver: " );
      msg.append( mpQuery->lastError().driverText() );
      msg.append( QString( "\nType: %1; Number: %2" )
                  .arg( mpQuery->lastError().type() )
                  .arg( mpQuery->lastError().number() )
                       );
      if( mpQuery->lastError().number() == SQLITE_BUSY )
      {
         msg.append( "\nShould implement retry!" );
      }
      msg.append( "\nQuery: " );

      QString query( mpQuery->lastQuery() );
      QMapIterator<QString, QVariant> i( mpQuery->boundValues() );
      while (i.hasNext())
      {
         i.next();
         query.replace( i.key(), i.value().toString() );
      }

      msg.append( query );
   }

   Satellite::send1( msg.toUtf8() );
}


void DatabaseThread::updateTrackInfo( const TrackInfo &trackInfo, bool allowinsert )
{
   prepare();

   if( trackInfo.mID )
   {
      mpQuery->prepare( "UPDATE slart_tracks SET Directory = :directory, FileName = :filename,"
                        " Artist = :artist, Title = :title, Album = :album, TrackNr = :tracknr,"
                        " Year = :year, Genre = :genre, PlayTime = :playtime, LastScanned = :lastscanned,"
                        " LastTagsRead = :lasttagsread,"
                        " TimesPlayed = :timesplayed, Volume = :volume, Folders = :folders, Flags = :flags"
                        " WHERE id = :id ;" );
      mpQuery->bindValue( ":id", trackInfo.mID );
   }
   else
   {
      if( trackInfo.mDirectory.isEmpty() || trackInfo.mFileName.isEmpty() || !allowinsert )
      {
         return;
      }
      mpQuery->prepare( "INSERT OR REPLACE INTO slart_tracks (Directory,FileName,Artist,Title,Album,"
                        "TrackNr,Year,Genre,PlayTime,LastScanned,LastTagsRead,TimesPlayed,Folders,Flags)"
                        " VALUES (:directory,:filename,:artist,:title,:album,:tracknr,:year,:genre,"
                        ":playtime,:lastscanned,:lasttagsread,:timesplayed,:folders,:flags);" );
   }
   mpQuery->bindValue( ":directory",    trackInfo.mDirectory );
   mpQuery->bindValue( ":filename",     trackInfo.mFileName );
   mpQuery->bindValue( ":artist",       trackInfo.mArtist );
   mpQuery->bindValue( ":title",        trackInfo.mTitle );
   mpQuery->bindValue( ":album",        trackInfo.mAlbum );
   if( trackInfo.mTrackNr < 0 )
   {
      mpQuery->bindValue( ":tracknr",   QVariant() );
   }
   else
   {
      mpQuery->bindValue( ":tracknr",   trackInfo.mTrackNr );
   }
   if( trackInfo.mYear < 0 )
   {
      mpQuery->bindValue( ":year",      QVariant() );
   }
   else
   {
      mpQuery->bindValue( ":year",      trackInfo.mYear );
   }
   mpQuery->bindValue( ":genre",        trackInfo.mGenre );
   mpQuery->bindValue( ":playtime",     trackInfo.mPlayTime );
   mpQuery->bindValue( ":lastscanned",  trackInfo.mLastScanned );
   mpQuery->bindValue( ":lasttagsread", trackInfo.mLastTagsRead );
   mpQuery->bindValue( ":timesplayed",  trackInfo.mTimesPlayed );
   mpQuery->bindValue( ":volume",       trackInfo.mVolume );
   mpQuery->bindValue( ":folders",      trackInfo.mFolders );
   mpQuery->bindValue( ":flags",        trackInfo.mFlags );
   if( !mpQuery->exec() )
   {
      logError();
   }
   mpQuery->clear();
}


void DatabaseThread::deleteTrackInfo( const TrackInfo &trackInfo )
{
   prepare();

   if( trackInfo.mID )
   {
      mpQuery->prepare( "DELETE FROM slart_tracks WHERE"
                        " id = :id ;" );
      mpQuery->bindValue( ":id", trackInfo.mID );
   }
   else
   {
      mpQuery->prepare( "DELETE FROM slart_tracks WHERE"
                        " Directory = :directory AND FileName = :filename ;" );
      mpQuery->bindValue( ":directory", trackInfo.mDirectory );
      mpQuery->bindValue( ":filename",  trackInfo.mFileName );
   }
   if( !mpQuery->exec() )
   {
      logError();
   }
   mpQuery->clear();
}




void DatabaseThread::insertFolder( const QString &folder )
{
   if( (folder == QChar(1)) || (folder == QChar(2)) )
   {
      return;
   }
   prepare();

   mpQuery->prepare( "INSERT OR REPLACE INTO slart_folders( Name ) VALUES ( :name );" );
   mpQuery->bindValue( ":name", folder );
   if( !mpQuery->exec() )
   {
      logError();
   }
   mpQuery->clear();
}


void DatabaseThread::deleteFolder( const QString &folder )
{
   prepare();

   mpQuery->prepare( "DELETE FROM slart_folders WHERE Name = :name;" );
   mpQuery->bindValue( ":name", folder );
   if( !mpQuery->exec() )
   {
      logError();
   }
   mpQuery->clear();
   // TODO: delete entries from slart_tracks
}


void DatabaseThread::rename( const QString &newName,
                             const QString &oldDirName, const QString &oldFileName )
{
#if 0
TRACESTART(Database::rename)
TRACEMSG << oldDirName << oldFileName << newName;
#endif
   if( oldFileName.isEmpty() )
   {
      /* rename a directory */
      mpQuery->prepare( "SELECT DISTINCT(Directory) FROM slart_tracks"
                        " WHERE Directory LIKE :directory ;" );
      mpQuery->bindValue( ":directory", oldDirName + "/%" );
      if( !mpQuery->exec() )
      {
         logError();
      }
      else
      {
         QStringList directories( oldDirName );
         QString     newDirName;
         while( mpQuery->next() )
         {
            directories << mpQuery->value(0).toString();
         }
         mpQuery->clear();
         foreach( const QString &directory, directories )
         {
            prepare();

            newDirName = directory;
            newDirName.replace( 0, oldDirName.size(), newName );
            mpQuery->prepare( "UPDATE slart_tracks SET Directory = :newDirName"
                              " WHERE Directory = :oldDirName;" );
            mpQuery->bindValue( ":newDirName", newDirName );
            mpQuery->bindValue( ":oldDirName", directory );
            if( !mpQuery->exec() )
            {
               logError();
            }
            else
            {
#if 0
TRACEMSG << "rows:" << mpQuery->numRowsAffected();
#endif
            }
            mpQuery->clear();
         }
      }
   }
   else
   {
      prepare();

      /* rename a file */
      mpQuery->prepare( "UPDATE slart_tracks SET FileName = :newName"
                        " WHERE Directory = :oldDirName AND FileName = :oldFileName;" );
      mpQuery->bindValue( ":newName", newName );
      mpQuery->bindValue( ":oldDirName", oldDirName );
      mpQuery->bindValue( ":oldFileName", oldFileName );
      if( !mpQuery->exec() )
      {
         logError();
      }
      else
      {
#if 0
TRACEMSG << "rows:" << mpQuery->numRowsAffected();
#endif
      }
      mpQuery->clear();
   }
}
