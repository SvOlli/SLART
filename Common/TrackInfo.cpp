/**
 * TrackInfo.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#include "Database.hpp"
#include "Trace.hpp"

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


void TrackInfo::clear()
{
   mID           = 0;
   mDirectory    = QString();
   mFileName     = QString();
   mArtist       = QString();
   mTitle        = QString();
   mAlbum        = QString();
   mTrackNr      = 0;
   mYear         = 0;
   mGenre        = QString();
   mPlayTime     = 0;
   mLastModified = 0;
   mTimesPlayed  = 0;
   mVolume       = 0.0;
   mFolders      = QString();
   mFlags        = 0;
}


void TrackInfo::setFlag( Flag flag, bool set )
{
   mFlags &= ~getFlagMask( flag );
   if( set )
   {
      mFlags |= (unsigned int)flag;
   }
}


bool TrackInfo::isFlagged( Flag flag )
{
   return (mFlags & getFlagMask( flag )) == (unsigned int)flag;
}


unsigned int TrackInfo::getFlagMask( Flag flag )
{
   switch( flag )
   {
      case ScannedWithPeak:
      case ScannedWithPower:
         return ScannedWithPeak | ScannedWithPower;
      case Favorite:
      case Unwanted:
         return Favorite | Unwanted;
      default:
         return 0;
   }
}


void TrackInfo::setFolder( const QString &folder, bool add )
{
   QStringList folders( mFolders.split('|', QString::SkipEmptyParts) );
   int i = folders.indexOf( folder );
   if( add )
   {
      if( i < 0 )
      {
         folders.append( QString(folder).replace('|','\\') );
         folders.sort();
      }
   }
   else
   {
      if( i >= 0 )
      {
         folders.removeAt( i );
      }
   }
   mFolders = folders.join( "|" );
   mFolders.prepend( "|" );
   mFolders.append( "|" );
}


QString TrackInfo::toString() const 
{
   return QString("id=%1,dir=%2,file=%3,artist=%4,title=%5,album=%6,trk=%7,year=%8,genre=%9,")
                  .arg(QString::number(mID), mDirectory, mFileName, mArtist, mTitle,
                       mAlbum, QString::number(mTrackNr), QString::number(mYear), mGenre)+
          QString("pt=%1,lm=%2,tp=%3,vol=%4,folders=%5,flags=%6")
                  .arg(QString::number(mPlayTime), QString::number(mLastModified),
                       QString::number(mTimesPlayed),QString::number(mVolume), mFolders,
                       QString::number(mFlags,16));
}

