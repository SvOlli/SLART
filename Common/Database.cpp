/**
 * Database.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#include "Database.hpp"

#include "Trace.hpp"

#include <stdlib.h>

Database::Database( const QString &fileName )
: mpSqlDB( 0 )
, mpQuery( 0 )
, mVersion( 0 )
{
   qsrand( time((time_t*)0) );
   
   mpSqlDB = new QSqlDatabase( QSqlDatabase::addDatabase( "QSQLITE" ) );
   
   if( fileName.isEmpty() )
   {
      char path[PATH_MAX];
      char *home = getenv( "HOME" );
      
      if( home )
      {
         strcpy( path, home );
         strcat( path, "/" );
      }
      else
      {
         path[0] = '\0';
      }
      strcat( path, ".slartdb" );
      
      mpSqlDB->setDatabaseName( path );
   }
   else
   {
      mpSqlDB->setDatabaseName( fileName );
   }
   
   if(!mpSqlDB->open())
   {
      // todo: some error handling
   }
   mpQuery = new QSqlQuery;
   
   if( mpQuery->exec( "SELECT value FROM slart_config WHERE key = 'Version';" ) )
   {
      if( mpQuery->next() )
      {
         mVersion = mpQuery->value(0).toUInt();
      }
   }
   else
   {
   }
   mpQuery->clear();
   
   if( !mVersion )
   {
      /* create database */
      QStringList initSQL;
      initSQL 
      << "CREATE TABLE slart_config (key VARCHAR PRIMARY KEY,"
                                    "value VARCHAR);"
      << "INSERT OR REPLACE INTO slart_config(key,value) VALUES ('Version',1);"
      
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
                                    "LastModified INTEGER,"
                                    "TimesPlayed INTEGER,"
                                    "Volume DOUBLE,"
                                    "Folders VARCHAR,"
                                    "Flags INTEGER);"
      << "CREATE UNIQUE INDEX slart_tracks_file ON slart_tracks (Directory,FileName);"
      << "CREATE INDEX slart_tracks_filename ON slart_tracks (FileName);"
      << "CREATE INDEX slart_tracks_artist ON slart_tracks (Artist);"
      << "CREATE INDEX slart_tracks_title ON slart_tracks (Title);"
      
      << "CREATE TABLE slart_folders (id INTEGER PRIMARY KEY,"
                                    "Name VARCHAR);"
      << "CREATE UNIQUE INDEX slart_folders_name ON slart_folders (Name);"
      
      ;
      
      for( int i = 0; i < initSQL.size(); i++ )
      {
         if(!mpQuery->exec( initSQL.at(i) ))
         {
         }
      }
   }
   else
   {
      /* upgrade database */
      switch(mVersion)
      {
      }
   }
   mpQuery->clear();
}


Database::~Database()
{
   if( mpQuery )
   {
      delete mpQuery;
   }
   if( mpSqlDB )
   {
      delete mpSqlDB;
   }
}


bool Database::getTrackInfo( TrackInfo *trackInfo, const QString &fileName )
{
   QString sql( "SELECT id,Directory,FileName,Artist,Title,Album,TrackNr,Year,Genre,"
                "PlayTime,LastModified,TimesPlayed,Volume,Folders,Flags FROM slart_tracks WHERE ");
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
      trackInfo->mLastModified = mpQuery->value(10).toUInt();
      trackInfo->mTimesPlayed  = mpQuery->value(11).toUInt();
      trackInfo->mVolume       = mpQuery->value(12).toDouble();
      trackInfo->mFolders      = mpQuery->value(13).toString();
      trackInfo->mFlags        = mpQuery->value(14).toUInt();
      
      mpQuery->clear();
      return true;
   }
   
   mpQuery->clear();
   return false;
}


unsigned int Database::getTrackInfoList( TrackInfoList *trackInfoList )
{
   if( trackInfoList )
   {
      mpQuery->prepare( "SELECT Directory,FileName,Artist,Title,Album,TrackNr,Year,"
                        "Genre,PlayTime,LastModified,TimesPlayed,Volume,Folders,Flags,id"
                        " FROM slart_tracks;" );
      if( !mpQuery->exec() )
      {
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
                                        mpQuery->value(11).toDouble(),
                                        mpQuery->value(12).toString(),
                                        mpQuery->value(13).toUInt() );
      }
      mpQuery->clear();
      return trackInfoList->size();
   }
   else
   {
      mpQuery->prepare( "SELECT COUNT(id) FROM slart_tracks;" );
      if( !mpQuery->exec() )
      {
      }
      mpQuery->next();
      mpQuery->clear();
      return mpQuery->value(0).toUInt();
   }
}


void Database::updateTrackInfo( const TrackInfo *trackInfo )
{
   if( trackInfo->mID )
   {
      mpQuery->prepare( "UPDATE slart_tracks SET Directory = :directory, FileName = :filename,"
                        " Artist = :artist, Title = :title, Album = :album, TrackNr = :tracknr,"
                        " Year = :year, Genre = :genre, PlayTime = :playtime, LastModified = :lastmodified,"
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
                        "TrackNr,Year,Genre,PlayTime,LastModified,TimesPlayed,Folders,Flags) VALUES"
                        " (:directory,:filename,:artist,:title,:album,:tracknr,:year,:genre,"
                        ":playtime,:lastmodified,:timesplayed,:folders,:flags);" );
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
   mpQuery->bindValue( ":lastmodified", trackInfo->mLastModified );
   mpQuery->bindValue( ":timesplayed",  trackInfo->mTimesPlayed );
   mpQuery->bindValue( ":volume",       trackInfo->mVolume );
   mpQuery->bindValue( ":folders",      trackInfo->mFolders );
   mpQuery->bindValue( ":flags",        trackInfo->mFlags );
   if( !mpQuery->exec() )
   {
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
   mpQuery->exec();
   mpQuery->clear();
}


QStringList Database::getFolders()
{
   QStringList folders;
   
   mpQuery->prepare( "SELECT Name FROM slart_folders ORDER BY Name;" );
   if( !mpQuery->exec() )
   {
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
   }
}


void Database::deleteFolder( const QString &folder )
{
   mpQuery->prepare( "DELETE FROM slart_folders WHERE Name = :name;" );
   mpQuery->bindValue( ":name", folder );
   if( !mpQuery->exec() )
   {
   }
}


bool Database::getTrack( TrackInfo *trackInfo, bool favorite, bool unplayed )
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
   
   sql.append( ";" );

   mpQuery->prepare( sql );
   if( !mpQuery->exec() )
   {
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
   }
   else
   {
      mpQuery->clear();
      return false;
   }
   
   return true;
}
