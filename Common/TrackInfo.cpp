/**
 * TrackInfo.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#include "Database.hpp"
#include "Trace.hpp"

TrackInfo::TrackInfo()
: mID( 0 )
, mDirectory()
, mFileName()
, mArtist()
, mTitle()
, mAlbum()
, mTrackNr( -1 )
, mYear( -1 )
, mGenre()
, mPlayTime( 0 )
, mLastScanned( 0 )
, mLastTagsRead( 0 )
, mTimesPlayed( 0 )
, mVolume( 0.0 )
, mFolders()
, mFlags( 0 )
{
}

TrackInfo::TrackInfo( const QString &directory, const QString &filename,
                      const QString &artist, const QString &title, const QString &album,
                      int tracknr, int year, const QString &genre,
                      unsigned int playtime, unsigned int lastscanned, unsigned int lasttagsread,
                      unsigned int timesplayed, double volume, 
                      const QString &folders, unsigned int flags, unsigned int id )
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
, mLastScanned( lastscanned )
, mLastTagsRead( lasttagsread )
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
, mLastScanned ( other.mLastScanned )
, mLastTagsRead( other.mLastTagsRead )
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
   mLastScanned  = other.mLastScanned;
   mLastTagsRead = other.mLastTagsRead;
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
   mTrackNr      = -1;
   mYear         = -1;
   mGenre        = QString();
   mPlayTime     = 0;
   mLastScanned  = 0;
   mLastTagsRead = 0;
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


QString TrackInfo::filePath() const
{
   return mDirectory + "/" + mFileName;
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


QStringList TrackInfo::getFolders()
{
   return mFolders.split('|', QString::SkipEmptyParts);
}


bool TrackInfo::isInFolder( const QString &folder )
{
   return mFolders.contains( QString("|") + folder + QString("|") );
}


QString TrackInfo::toString() const 
{
   return QString("id=%1,dir=%2,file=%3,artist=%4,title=%5,album=%6,trk=%7,year=%8,genre=%9,")
                  .arg(QString::number(mID), mDirectory, mFileName, mArtist, mTitle,
                       mAlbum, QString::number(mTrackNr), QString::number(mYear), mGenre)+
          QString("pt=%1,ls=%2,lt=%3,tp=%4,vol=%5,folders=%6,flags=%7")
                  .arg(QString::number(mPlayTime), QString::number(mLastScanned),
                       QString::number(mLastTagsRead),QString::number(mTimesPlayed),
                       QString::number(mVolume), mFolders, QString::number(mFlags,16));
}


QString TrackInfo::sec2minsec( int seconds )
{
   if( seconds < 0 )
   {
      seconds = 0;
   }
   int minutes = seconds / 60;
   seconds %= 60;
   QString time( QString::number( minutes ) );
   if( seconds < 10 )
   {
      time.append( ":0" );
   }
   else
   {
      time.append( ":" );
   }
   time.append( QString::number( seconds ) );

   return time;
}


QString TrackInfo::valueByKey( const QString &key ) const
{
   if( key.toUpper() == "ID" )          return QString::number( mID );
   if( key.toUpper() == "DIRECTORY" )   return mDirectory;
   if( key.toUpper() == "FILENAME" )    return mFileName;
   if( key.toUpper() == "ARTIST" )      return mArtist;
   if( key.toUpper() == "TITLE" )       return mTitle;
   if( key.toUpper() == "ALBUM" )       return mAlbum;
   if( key.toUpper() == "TRACKNUMBER" ) return QString::number( mTrackNr );
   if( key.toUpper() == "DATE" )        return QString::number( mYear );
   if( key.toUpper() == "GENRE" )       return mGenre;
   if( key.toUpper() == "PLAYTIME" )    return sec2minsec( mPlayTime );
   if( key.toUpper() == "TIMESPLAYED" ) return QString::number( mTimesPlayed );
   if( key.toUpper() == "FOLDERS" )     return mFolders;
   return QString();
}


QString TrackInfo::displayString( const QString &pattern ) const 
{
   if( pattern.isEmpty() || mArtist.isEmpty() || mTitle.isEmpty() )
   {
      return mFileName;
   }
   
   QStringList parts( pattern.split( "|", QString::SkipEmptyParts ) );
   QString filename;
   int i;

   for( i = 0; i < parts.size() ; i++ )
   {
      switch( parts.at(i).at(0).unicode() )
      {
         case '$':
            filename.append( valueByKey( parts.at(i).toUpper().mid(1) ) );
            break;
         case '#':
            {
               if( parts.at(i).size() < 2 )
               {
                  break;
               }
               bool ok;
               int size = parts.at(i).mid(1,1).toInt( &ok );
                  
               if( ok )
               {
                  int number = valueByKey( parts.at(i).toUpper().mid(2) ).toInt( &ok );
                  
                  if( !ok || (number < 0) ) break;
                  filename.append( QString::number(1000000000 + number).right(size) );
               }
               else
               {
                  switch( parts.at(i).at(1).unicode() )
                  {
                     case '#':
                     case '$':
                        filename.append( parts.at(i).at(1) );
                        break;
                     case '/':
                        filename.append( "|" );
                        break;
                     case 'n':
                        filename.append( "\n" );
                        break;
                     default:
                        break;
                  }
               }
            }
            break;
         default:
            filename.append( parts.at(i) );
            break;
      }
   }
   
   return filename;
}
