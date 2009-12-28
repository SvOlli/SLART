/**
 * src/libs/Common/TrackInfo.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef TRACKINFO_HPP
#define TRACKINFO_HPP TRACKINFO_HPP

#include <QString>


class TrackInfo
{
public:
   enum Flag {
      ScannedWithPeak  = 0x00000001,
      ScannedWithPower = 0x00000002,
      Unwanted         = 0x00000004,
      Favorite         = 0x00000008,
      None             = 0x00000000
   };
   
   TrackInfo();
   TrackInfo( const TrackInfo &other );
   TrackInfo &operator=( const TrackInfo &other );
   TrackInfo( const QString &directory,
              const QString &filename,
              const QString &artist,
              const QString &title,
              const QString &album,
              int            tracknr,
              int            year,
              const QString &genre,
              unsigned int   playtime,
              unsigned int   lastscanned,
              unsigned int   lasttagsread,
              unsigned int   timesplayed = 0,
              double         volume = 0.0,
              const QString &folders = QString(),
              unsigned int   flags = 0,
              unsigned int   id = 0 );
   
   unsigned int mID;
   QString      mDirectory;
   QString      mFileName;
   QString      mArtist;
   QString      mTitle;
   QString      mAlbum;
   int          mTrackNr;
   int          mYear;
   QString      mGenre;
   unsigned int mPlayTime;
   unsigned int mLastScanned;
   unsigned int mLastTagsRead;
   unsigned int mTimesPlayed;
   double       mVolume;
   QString      mFolders;
   unsigned int mFlags;
   
   /* clear all data */
   void clear();
   
   /* set or unset a flag */
   void setFlag( Flag flag, bool set );
   /* check if a flag is set */
   bool isFlagged( Flag flag );
   /* for combined flags get the mask */
   unsigned int getFlagMask( Flag flag );
   
   /* add or remove the track to a folder */
   void setFolder( const QString &folder, bool add );
   /* check if the track is in a folder */
   bool isInFolder( const QString &folder );
   /* get a list of all folders the track is in */
   QStringList getFolders();
   
   /* return full path of file */
   QString filePath() const;
   /* put it all together in a string for debugging purposes */
   QString toString() const;
   /* format to a string by patterns */
   QString displayString( const QString &pattern ) const;
   
   /* convert seconds to a 0:00 timecode */
   static QString sec2minsec( int seconds );
   
private:
   /* subroutine for displayString */
   QString valueByKey( const QString &key ) const;
};

typedef QList<TrackInfo> TrackInfoList;


#endif
