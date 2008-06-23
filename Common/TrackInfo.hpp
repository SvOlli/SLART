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
   TrackInfo();
   TrackInfo( const TrackInfo &other );
   TrackInfo &operator=( const TrackInfo &other );
   TrackInfo( const QString &directory,
              const QString &filename,
              const QString &artist,
              const QString &title,
              const QString &album,
              unsigned int   tracknr,
              unsigned int   year,
              const QString &genre,
              unsigned int   playtime,
              unsigned int   lastmodified,
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
   unsigned int mTrackNr;
   unsigned int mYear;
   QString      mGenre;
   unsigned int mPlayTime;
   unsigned int mLastModified;
   unsigned int mTimesPlayed;
   double       mVolume;
   QString      mFolders;
   unsigned int mFlags;
};

typedef QList<TrackInfo> TrackInfoList;

#endif
