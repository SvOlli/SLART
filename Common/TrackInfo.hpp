/**
 * TrackInfo.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
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
   
   /*  */
   void clear();
   
   /*  */
   void setFlag( Flag flag, bool set );
   /*  */
   bool isFlagged( Flag flag );
   /*  */
   unsigned int getFlagMask( Flag flag );
   
   /*  */
   void setFolder( const QString &folder, bool add );
   /*  */
   bool isInFolder( const QString &folder );
   /*  */
   QStringList getFolders();
   
   /*  */
   QString filePath() const;
   /*  */
   QString toString() const;
   /*  */
   QString displayString( const QString &pattern ) const;

   /* convert seconds to a 0:00 timecode */
   static QString sec2minsec( int seconds );

private:
   /*  */
   QString valueByKey( const QString &key ) const;
};

typedef QList<TrackInfo> TrackInfoList;


#endif
