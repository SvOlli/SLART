/**
 * Database.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#include "Database.hpp"

#include "Trace.hpp"

#include <stdlib.h>

Database::Database()
: mpSqlDB(0)
, mpQuery(0)
, mVersion(0)
{
   mpSqlDB = new QSqlDatabase( QSqlDatabase::addDatabase( "QSQLITE" ) );
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
                                    "Flags INTEGER);"
      << "CREATE UNIQUE INDEX slart_tracks_file ON slart_tracks(directory,filename);"
      << "CREATE INDEX slart_tracks_filename ON slart_tracks(filename);"
      << "CREATE INDEX slart_tracks_artist ON slart_tracks(artist);"
      << "CREATE INDEX slart_tracks_title ON slart_tracks(title);"
      
      ;
      
      for( int i = 0; i < initSQL.size(); i++ )
      {
         if(!mpQuery->exec( initSQL.at(i) ))
         {
         }
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


bool Database::getTrackInfoByFileName( TrackInfo *trackInfo, const QString &fileName )
{
   int fileNameStart = fileName.lastIndexOf('/');
   
   mpQuery->prepare( "SELECT id,Directory,FileName,Artist,Title,Album,TrackNr,Year,Genre,"
                   "PlayTime,LastModified,TimesPlayed,Volume,Flags FROM slart_tracks"
                   " WHERE Directory = :directory AND FileName = :fileName ;" );
   mpQuery->bindValue( ":directory", fileName.left(fileNameStart)/*.replace("'","''" )*/ );
   mpQuery->bindValue( ":fileName", fileName.mid(fileNameStart+1)/*.replace("'","''" )*/ );   
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
      trackInfo->mVolume       = mpQuery->value(11).toDouble();
      trackInfo->mFlags        = mpQuery->value(12).toUInt();
      
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
                        "Genre,PlayTime,LastModified,TimesPlayed,Volume,Flags,id"
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
                                        mpQuery->value(12).toUInt(),
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
                        " TimesPlayed = :timesplayed, Volume = :volume, Flags = :flags"
                        " WHERE id = :id ;" );
      mpQuery->bindValue( ":id", trackInfo->mID );
   }
   else
   {
      mpQuery->prepare( "INSERT OR REPLACE INTO slart_tracks (Directory,FileName,Artist,Title,Album,"
                        "TrackNr,Year,Genre,PlayTime,LastModified,TimesPlayed,Flags) values"
                        " (:directory,:filename,:artist,:title,:album,:tracknr,:year,:genre,"
                        ":playtime,:lastmodified,:timesplayed,:flags);" );
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
