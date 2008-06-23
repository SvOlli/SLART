/**
 * TrackInfo.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#include "Database.hpp"

TrackInfo::TrackInfo()
{
}

TrackInfo::TrackInfo( const QString &directory, const QString &filename,
                      const QString &artist, const QString &title, const QString &album,
                      unsigned int tracknr, unsigned int year, const QString &genre,
                      unsigned int playtime, unsigned int lastmodified, unsigned int timesplayed,
                      double volume, const QString &folders, unsigned int flags, unsigned int id )
: mID( id )
, mDirectory( directory )
, mFileName( filename )
, mArtist( artist )
, mTitle( title )
, mAlbum( album )
, mTrackNr( tracknr )
, mYear( year )
, mGenre( genre )
, mPlayTime( playtime )
, mLastModified( lastmodified )
, mTimesPlayed( timesplayed )
, mVolume( volume )
, mFolders( folders )
, mFlags( flags )
{
   
}

TrackInfo::TrackInfo( const TrackInfo &other )
: mID( other.mID )
, mDirectory   ( other.mDirectory )
, mFileName    ( other.mFileName )
, mArtist      ( other.mArtist )
, mTitle       ( other.mTitle )
, mAlbum       ( other.mAlbum )
, mTrackNr     ( other.mTrackNr )
, mYear        ( other.mYear )
, mGenre       ( other.mGenre )
, mPlayTime    ( other.mPlayTime )
, mLastModified( other.mLastModified )
, mTimesPlayed ( other.mTimesPlayed )
, mVolume      ( other.mVolume )
, mFolders     ( other.mFolders )
, mFlags       ( other.mFlags )
{
}

TrackInfo &TrackInfo::operator=( const TrackInfo &other )
{
   mID           = other.mID;
   mDirectory    = other.mDirectory;
   mFileName     = other.mFileName;
   mArtist       = other.mArtist;
   mTitle        = other.mTitle;
   mAlbum        = other.mAlbum;
   mTrackNr      = other.mTrackNr;
   mYear         = other.mYear;
   mGenre        = other.mGenre;
   mPlayTime     = other.mPlayTime;
   mLastModified = other.mLastModified;
   mTimesPlayed  = other.mTimesPlayed;
   mVolume       = other.mVolume;
   mFolders      = other.mFolders;
   mFlags        = other.mFlags;
   
   return *this;
}
