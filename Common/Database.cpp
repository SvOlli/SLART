/**
 * Database.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#include "Database.hpp"
#include "MySettings.hpp"
#include <QApplication>
#include <QMessageBox>

#include "Trace.hpp"


Database::Database( const QString &fileName )
: mpSqlDB( 0 )
, mpQuery( 0 )
, mDatabaseVersion( 0 )
, mCodeVersion( 1 )
{
   int i;
   qsrand( time((time_t*)0) );
   
   mpSqlDB = new QSqlDatabase( QSqlDatabase::addDatabase( "QSQLITE" ) );
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
      // todo: some error handling
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
      
      for( i = 0; i < initSQL.size(); i++ )
      {
         if(!mpQuery->exec( initSQL.at(i) ))
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
         for( i = 0; i < updateSQL.size(); i++ )
         {
            if(!mpQuery->exec( updateSQL.at(i) ))
            {
               logError();
            }
            mpQuery->clear();
         }
      }
   }
   mpQuery->clear();
}


Database::~Database()
{
   if( mpQuery )
   {
      mpQuery->clear();
      delete mpQuery;
   }
   if( mpSqlDB )
   {
      mpSqlDB->commit();
      mpSqlDB->close();
      /* clean up database, line below doesn't work without annoying warning */
      //mpSqlDB->removeDatabase( mpSqlDB->connectionName() );
      /* use this one instead, found on:
         http://lists.trolltech.com/qt4-preview-feedback/2005-08/thread00142-0.html#msg00143 */
      *mpSqlDB = QSqlDatabase();
      delete mpSqlDB;
   }
}


bool Database::getTrackInfo( TrackInfo *trackInfo, const QString &fileName )
{
   QString sql( "SELECT id,Directory,FileName,Artist,Title,Album,"
                "CASE WHEN TrackNr NOTNULL THEN TrackNr ELSE -1 END,"
                "CASE WHEN Year NOTNULL THEN Year ELSE -1 END,"
                "Genre,PlayTime,LastScanned,LastTagsRead,TimesPlayed,Volume,Folders,Flags"
                " FROM slart_tracks WHERE ");
   if( fileName.isEmpty() )
   {
      sql.append( "id = :id ;" );
      mpQuery->prepare( sql );
      if( !trackInfo )
      {
         return false;
      }
      mpQuery->bindValue( ":id", trackInfo->mID );
   }
   else
   {
      int fileNameStart = fileName.lastIndexOf('/');
      sql.append( "Directory = :directory AND FileName = :fileName ;" );
      
      mpQuery->prepare( sql );
      mpQuery->bindValue( ":directory", fileName.left(fileNameStart) );
      mpQuery->bindValue( ":fileName", fileName.mid(fileNameStart+1) );   
   }
   if( !mpQuery->exec() )
   {
      logError();
   }
   
   if( mpQuery->next() )
   {
      if( trackInfo )
      {
         trackInfo->mID           = mpQuery->value( 0).toUInt();
         trackInfo->mDirectory    = mpQuery->value( 1).toString();
         trackInfo->mFileName     = mpQuery->value( 2).toString();
         trackInfo->mArtist       = mpQuery->value( 3).toString();
         trackInfo->mTitle        = mpQuery->value( 4).toString();
         trackInfo->mAlbum        = mpQuery->value( 5).toString();
         trackInfo->mTrackNr      = mpQuery->value( 6).toInt();
         trackInfo->mYear         = mpQuery->value( 7).toInt();
         trackInfo->mGenre        = mpQuery->value( 8).toString();
         trackInfo->mPlayTime     = mpQuery->value( 9).toUInt();
         trackInfo->mLastScanned  = mpQuery->value(10).toUInt();
         trackInfo->mLastTagsRead = mpQuery->value(11).toUInt();
         trackInfo->mTimesPlayed  = mpQuery->value(12).toUInt();
         trackInfo->mVolume       = mpQuery->value(13).toDouble();
         trackInfo->mFolders      = mpQuery->value(14).toString();
         trackInfo->mFlags        = mpQuery->value(15).toUInt();
      }
      
      mpQuery->clear();
      return true;
   }
   else
   {
      if( trackInfo )
      {
         trackInfo->clear();
      }
      
      mpQuery->clear();
      return false;
   }
}


int Database::getTrackInfoList( TrackInfoList *trackInfoList, const QString &search )
{
   if( trackInfoList )
   {
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
      if( !mpQuery->exec() )
      {
         logError();
      }
      
      trackInfoList->clear();
      while( mpQuery->next() )
      {
         (*trackInfoList) << TrackInfo( mpQuery->value( 0).toString(),
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
      return trackInfoList->size();
   }
   else
   {
      mpQuery->prepare( "SELECT COUNT(id) FROM slart_tracks;" );
      if( !mpQuery->exec() )
      {
         logError();
      }
      mpQuery->next();
      int tracks = mpQuery->value(0).toInt();
      mpQuery->clear();
      return tracks;
   }
}


void Database::updateTrackInfo( const TrackInfo *trackInfo, bool allowinsert )
{
   if( trackInfo->mID )
   {
      mpQuery->prepare( "UPDATE slart_tracks SET Directory = :directory, FileName = :filename,"
                        " Artist = :artist, Title = :title, Album = :album, TrackNr = :tracknr,"
                        " Year = :year, Genre = :genre, PlayTime = :playtime, LastScanned = :lastscanned,"
                        " LastTagsRead = :lasttagsread,"
                        " TimesPlayed = :timesplayed, Volume = :volume, Folders = :folders, Flags = :flags"
                        " WHERE id = :id ;" );
      mpQuery->bindValue( ":id", trackInfo->mID );
   }
   else
   {
      if( trackInfo->mDirectory.isEmpty() || trackInfo->mFileName.isEmpty() || !allowinsert )
      {
         return;
      }
      mpQuery->prepare( "INSERT OR REPLACE INTO slart_tracks (Directory,FileName,Artist,Title,Album,"
                        "TrackNr,Year,Genre,PlayTime,LastScanned,LastTagsRead,TimesPlayed,Folders,Flags)"
                        " VALUES (:directory,:filename,:artist,:title,:album,:tracknr,:year,:genre,"
                        ":playtime,:lastscanned,:lasttagsread,:timesplayed,:folders,:flags);" );
   }
   mpQuery->bindValue( ":directory",    trackInfo->mDirectory );
   mpQuery->bindValue( ":filename",     trackInfo->mFileName );
   mpQuery->bindValue( ":artist",       trackInfo->mArtist );
   mpQuery->bindValue( ":title",        trackInfo->mTitle );
   mpQuery->bindValue( ":album",        trackInfo->mAlbum );
   if( trackInfo->mTrackNr < 0 )
   {
      mpQuery->bindValue( ":tracknr",   QVariant() );
   }
   else
   {
      mpQuery->bindValue( ":tracknr",   trackInfo->mTrackNr );
   }
   if( trackInfo->mYear < 0 )
   {
      mpQuery->bindValue( ":year",      QVariant() );
   }
   else
   {
      mpQuery->bindValue( ":year",      trackInfo->mYear );
   }
   mpQuery->bindValue( ":genre",        trackInfo->mGenre );
   mpQuery->bindValue( ":playtime",     trackInfo->mPlayTime );
   mpQuery->bindValue( ":lastscanned",  trackInfo->mLastScanned );
   mpQuery->bindValue( ":lasttagsread", trackInfo->mLastTagsRead );
   mpQuery->bindValue( ":timesplayed",  trackInfo->mTimesPlayed );
   mpQuery->bindValue( ":volume",       trackInfo->mVolume );
   mpQuery->bindValue( ":folders",      trackInfo->mFolders );
   mpQuery->bindValue( ":flags",        trackInfo->mFlags );
   if( !mpQuery->exec() )
   {
      logError();
   }
   mpQuery->clear();
}


void Database::deleteTrackInfo( const TrackInfo *trackInfo )
{
   if( trackInfo->mID )
   {
      mpQuery->prepare( "DELETE FROM slart_tracks WHERE"
                        " id = :id ;" );
      mpQuery->bindValue( ":id", trackInfo->mID );
   }
   else
   {
      mpQuery->prepare( "DELETE FROM slart_tracks WHERE"
                        " Directory = :directory AND FileName = :filename ;" );
      mpQuery->bindValue( ":directory",    trackInfo->mDirectory );
      mpQuery->bindValue( ":filename",     trackInfo->mFileName );
   }
   if( !mpQuery->exec() )
   {
      logError();
   }
   mpQuery->clear();
}


QStringList Database::getFolders()
{
   QStringList folders;
   
   mpQuery->prepare( "SELECT Name FROM slart_folders ORDER BY Name;" );
   if( !mpQuery->exec() )
   {
      logError();
   }
   while( mpQuery->next() )
   {
      folders << mpQuery->value(0).toString();
   }
   mpQuery->clear();
   
   return folders;
}


QStringList Database::getFolder( const QString &folder )
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
      if( !mpQuery->exec() )
      {
         logError();
      }
      while( mpQuery->next() )
      {
         folders << mpQuery->value(0).toString() + '/' + mpQuery->value(1).toString();
      }
      mpQuery->clear();
   }
   
   return folders;
}


void Database::insertFolder( const QString &folder )
{
   mpQuery->prepare( "INSERT OR REPLACE INTO slart_folders( Name ) VALUES ( :name );" );
   mpQuery->bindValue( ":name", folder );
   if( !mpQuery->exec() )
   {
      logError();
   }
   mpQuery->clear();
}


void Database::deleteFolder( const QString &folder )
{
   mpQuery->prepare( "DELETE FROM slart_folders WHERE Name = :name;" );
   mpQuery->bindValue( ":name", folder );
   if( !mpQuery->exec() )
   {
      logError();
   }
   mpQuery->clear();
   // TODO: delete entries from slart_tracks
}


bool Database::getRandomTrack( TrackInfo *trackInfo, bool favorite,
                               bool leastplayed, const QString &folder )
{
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
      
      trackInfo->mID = mpQuery->value(0).toUInt();
      mpQuery->clear();
      getTrackInfo( trackInfo );
      
      return true;
   }
   else
   {
      mpQuery->clear();
      return false;
   }
}


void Database::logError( const QString &note )
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
      msg.append( "\nQuery: " );
      msg.append( mpQuery->lastQuery() );
   }
   MySettings().sendUdpMessage( msg, QString("Innuendo") );
}


bool Database::exists()
{
   return QFileInfo( getDatabaseFileName() ).isFile();
}


QString Database::getDatabaseFileName()
{
#ifdef _WIN32
   QString slartdb( "/slart.db" );
#else
   QString slartdb( "/.slartdb" );
#endif
   return QDir::homePath() + slartdb;
}


void Database::rename( const QString &newName,
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
         for( int i = 0; i < directories.size(); i++ )
         {
            newDirName = directories.at(i);
            newDirName.replace( 0, oldDirName.size(), newName );
            mpQuery->prepare( "UPDATE slart_tracks SET Directory = :newDirName"
                              " WHERE Directory = :oldDirName;" );
            mpQuery->bindValue( ":newDirName", newDirName );
            mpQuery->bindValue( ":oldDirName", directories.at(i) );
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
