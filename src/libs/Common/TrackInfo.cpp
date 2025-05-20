/*
 * src/libs/Common/TrackInfo.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
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
, mGroups()
, mFlags( 0 )
{
}

TrackInfo::TrackInfo( const QString &directory, const QString &filename,
                      const QString &artist, const QString &title, const QString &album,
                      int tracknr, int year, const QString &genre,
                      unsigned int playtime, unsigned int lastscanned, unsigned int lasttagsread,
                      unsigned int timesplayed, double volume,
                      const QString &groups, unsigned int flags, unsigned int id )
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
, mGroups( groups )
, mFlags( flags )
{

}

TrackInfo::TrackInfo( const TrackInfo &that )
: mID( that.mID )
, mDirectory   ( that.mDirectory )
, mFileName    ( that.mFileName )
, mArtist      ( that.mArtist )
, mTitle       ( that.mTitle )
, mAlbum       ( that.mAlbum )
, mTrackNr     ( that.mTrackNr )
, mYear        ( that.mYear )
, mGenre       ( that.mGenre )
, mPlayTime    ( that.mPlayTime )
, mLastScanned ( that.mLastScanned )
, mLastTagsRead( that.mLastTagsRead )
, mTimesPlayed ( that.mTimesPlayed )
, mVolume      ( that.mVolume )
, mGroups     ( that.mGroups )
, mFlags       ( that.mFlags )
{
}


TrackInfo &TrackInfo::operator=( const TrackInfo &that )
{
   if( this == &that )
   {
      return *this;
   }
   mID           = that.mID;
   mDirectory    = that.mDirectory;
   mFileName     = that.mFileName;
   mArtist       = that.mArtist;
   mTitle        = that.mTitle;
   mAlbum        = that.mAlbum;
   mTrackNr      = that.mTrackNr;
   mYear         = that.mYear;
   mGenre        = that.mGenre;
   mPlayTime     = that.mPlayTime;
   mLastScanned  = that.mLastScanned;
   mLastTagsRead = that.mLastTagsRead;
   mTimesPlayed  = that.mTimesPlayed;
   mVolume       = that.mVolume;
   mGroups      = that.mGroups;
   mFlags        = that.mFlags;

   return *this;
}


bool TrackInfo::operator==( const TrackInfo &that ) const
{
   return
   (
         (mID           == that.mID )
      && (mDirectory    == that.mDirectory )
      && (mFileName     == that.mFileName )
      && (mArtist       == that.mArtist )
      && (mTitle        == that.mTitle )
      && (mAlbum        == that.mAlbum )
      && (mTrackNr      == that.mTrackNr )
      && (mYear         == that.mYear )
      && (mGenre        == that.mGenre )
      && (mPlayTime     == that.mPlayTime )
      && (mLastScanned  == that.mLastScanned )
      && (mLastTagsRead == that.mLastTagsRead )
      && (mTimesPlayed  == that.mTimesPlayed )
      && (mVolume       == that.mVolume )
      && (mGroups      == that.mGroups )
      && (mFlags        == that.mFlags )
   );
}

bool TrackInfo::operator!=( const TrackInfo &that ) const
{
   return !operator==( that );
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
   mGroups      = QString();
   mFlags        = 0;
}


bool TrackInfo::isInDatabase() const
{
   return (mID > 0);
}


void TrackInfo::setFlag( Flag flag, bool set )
{
   mFlags &= ~getFlagMask( flag );
   if( set )
   {
      mFlags |= (unsigned int)flag;
   }
}


bool TrackInfo::isFlagged( Flag flag ) const
{
   return (mFlags & getFlagMask( flag )) == (unsigned int)flag;
}


unsigned int TrackInfo::getFlagMask( Flag flag ) const
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


void TrackInfo::setGroup( const QString &group, bool add )
{
   QStringList groupList( mGroups.split('|', Qt::SkipEmptyParts) );
   int i = groupList.indexOf( group );
   if( add )
   {
      if( i < 0 )
      {
         groupList.append( QString(group).replace('|','\\') );
         groupList.sort();
      }
   }
   else
   {
      if( i >= 0 )
      {
         groupList.removeAt( i );
      }
   }
   if( groupList.count() )
   {
      mGroups = groupList.join( "|" );
      mGroups.prepend( "|" );
      mGroups.append( "|" );
   }
   else
   {
      mGroups.clear();
   }
}


QStringList TrackInfo::getGroups() const
{
   return mGroups.split('|', Qt::SkipEmptyParts);
}


bool TrackInfo::isInGroup( const QString &group ) const
{
   return mGroups.contains( QString("|") + group + QString("|") );
}


QString TrackInfo::toString() const
{
   return QString("id=%1,dir=%2,file=%3,artist=%4,title=%5,album=%6,trk=%7,year=%8,genre=%9,")
                  .arg(QString::number(mID), mDirectory, mFileName, mArtist, mTitle,
                       mAlbum, QString::number(mTrackNr), QString::number(mYear), mGenre)+
          QString("pt=%1,ls=%2,lt=%3,tp=%4,vol=%5,groups=%6,flags=%7")
                  .arg(QString::number(mPlayTime), QString::number(mLastScanned),
                       QString::number(mLastTagsRead),QString::number(mTimesPlayed),
                       QString::number(mVolume), mGroups, QString::number(mFlags,16));
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
   if( key.toUpper() == "FOLDERS" )     return mGroups;
   return QString();
}


QString TrackInfo::displayString( const QString &pattern ) const
{
   if( pattern.isEmpty() || mArtist.isEmpty() || mTitle.isEmpty() )
   {
      return mFileName;
   }

   QStringList parts( pattern.split( "|", Qt::SkipEmptyParts ) );
   QString filename;

   foreach( const QString &part, parts )
   {
      switch( part.at(0).unicode() )
      {
         case '$':
            filename.append( valueByKey( part.toUpper().mid(1) ) );
            break;
         case '#':
            {
               if( part.size() < 2 )
               {
                  break;
               }
               bool ok;
               int size = part.mid(1,1).toInt( &ok );

               if( ok )
               {
                  int number = valueByKey( part.toUpper().mid(2) ).toInt( &ok );

                  if( !ok || (number < 0) ) break;
                  filename.append( QString::number(1000000000 + number).right(size) );
               }
               else
               {
                  switch( part.at(1).unicode() )
                  {
                     case '#':
                     case '$':
                        filename.append( part.at(1) );
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
            filename.append( part );
            break;
      }
   }

   return filename;
}
