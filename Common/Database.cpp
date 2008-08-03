/**
 * Database.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#include "Database.hpp"
#include "MySettings.hpp"
#include <QApplication>

#include "Trace.hpp"

#include <stdlib.h>

Database::Database( const QString &fileName )
: mpSqlDB( 0 )
, mpQuery( 0 )
, mDatabaseVersion( 0 )
, mCodeVersion( 1 )
{
   int i;
   qsrand( time((time_t*)0) );
   
   mpSqlDB = new QSqlDatabase( QSqlDatabase::addDatabase( "QSQLITE" ) );
   
   if( fileName.isEmpty() )
   {
      char *path = getDatabaseFileName();
      mpSqlDB->setDatabaseName( path );
      free( path );
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
      mpSqlDB->removeDatabase( "QSQLITE" );
      delete mpSqlDB;
   }
}


bool Database::getTrackInfo( TrackInfo *trackInfo, const QString &fileName )
{
   QString sql( "SELECT id,Directory,FileName,Artist,Title,Album,TrackNr,Year,Genre,"
                "PlayTime,LastScanned,LastTagsRead,TimesPlayed,Volume,Folders,Flags"
                " FROM slart_tracks WHERE ");
   if( fileName.isEmpty() )
   {
      sql.append( "id = :id ;" );
      mpQuery->prepare( sql );
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
      trackInfo->mID           = mpQuery->value( 0).toUInt();
      trackInfo->mDirectory    = mpQuery->value( 1).toString();
      trackInfo->mFileName     = mpQuery->value( 2).toString();
      trackInfo->mArtist       = mpQuery->value( 3).toString();
      trackInfo->mTitle        = mpQuery->value( 4).toString();
      trackInfo->mAlbum        = mpQuery->value( 5).toString();
      trackInfo->mTrackNr      = mpQuery->value( 6).toUInt();
      trackInfo->mYear         = mpQuery->value( 7).toUInt();
      trackInfo->mGenre        = mpQuery->value( 8).toString();
      trackInfo->mPlayTime     = mpQuery->value( 9).toUInt();
      trackInfo->mLastScanned  = mpQuery->value(10).toUInt();
      trackInfo->mLastTagsRead = mpQuery->value(11).toUInt();
      trackInfo->mTimesPlayed  = mpQuery->value(12).toUInt();
      trackInfo->mVolume       = mpQuery->value(13).toDouble();
      trackInfo->mFolders      = mpQuery->value(14).toString();
      trackInfo->mFlags        = mpQuery->value(15).toUInt();
      
      mpQuery->clear();
      return true;
   }
   else
   {
      trackInfo->clear();
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
      mpQuery->prepare( "SELECT Directory,FileName,Artist,Title,Album,TrackNr,Year,Genre,"
                        "PlayTime,LastScanned,LastTagsRead,TimesPlayed,Volume,Folders,Flags,id"
                        " FROM slart_tracks WHERE FileName LIKE :filename ;" );
      mpQuery->bindValue( ":filename", sqlSearch );
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
                                        mpQuery->value( 5).toUInt(),
                                        mpQuery->value( 6).toUInt(),
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


void Database::updateTrackInfo( const TrackInfo *trackInfo )
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
      if( trackInfo->mDirectory.isEmpty() || trackInfo->mFileName.isEmpty() )
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
   mpQuery->bindValue( ":tracknr",      trackInfo->mTrackNr );
   mpQuery->bindValue( ":year",         trackInfo->mYear );
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
                               bool unplayed, const QString &folder )
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
   
   if( unplayed )
   {
      sql.append( " AND TimesPlayed = 0" );
   }
   
   if( !folder.isEmpty() )
   {
      sql.append( " AND Folders LIKE = '%|" );
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
   for( rows = 0; mpQuery->next(); rows++);
   
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


bool Database::databaseExists()
{
   char *fileName = getDatabaseFileName();
   QFileInfo qfi( fileName );
   free( fileName );
   return qfi.isFile();
}


char *Database::getDatabaseFileName()
{
   const char *dotslartdb = ".slartdb";
   char *filePath = 0;
   char *home = getenv( "HOME" );
   
   if( home )
   {
      filePath = (char*)malloc( strlen(home) + 1 + strlen(dotslartdb) );
      strcpy( filePath, home );
      strcat( filePath, "/" );
      strcat( filePath, dotslartdb );
      return( filePath );
   }
   else
   {
      return strdup( dotslartdb );
   }
}


