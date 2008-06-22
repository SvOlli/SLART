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
                      unsigned int playtime, unsigned int lastmodified,
                      unsigned int id, unsigned int timesplayed, unsigned int flags )
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
, mFlags( flags )
{
   
}
