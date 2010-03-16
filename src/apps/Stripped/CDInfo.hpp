/**
 * src/apps/Stripped/CDInfo.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef CDINFO_HPP
#define CDINFO_HPP CDINFO_HPP

/* base class */

/* system headers */

/* Qt headers */
#include <QString>

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */

/* forward declaration of local classes */


class CDInfo
{
public:
   CDInfo();
   virtual ~CDInfo();

   /* reset data */
   void clear();

   /* set the global disc data (also resets all other data) */
   void setDisc( int tracks, int startSector, int endSector );
   /* set the data for a track */
   void setTrack( int track, int startSector, int endSector,
                  bool isAudio, bool copyProhib, bool preEmp, bool twoChannels );
   /*  */
   void setArtist( int track = -1, const QString &artist = QString(), bool append = false );
   /* set the title for a track */
   void setTitle( int track = -1, const QString &title = QString(), bool append = false );
   /*  */
   void setGenre( const QString &genre = QString(), bool append = false );
   /*  */
   void setYear( int year );
   /*  */
   void splitTitle( const QString &separator );
   /* calculate the cddb discid */
   void calcCddbDiscID();
   /* return the cddb discid */
   QString cddbDiscID();
   /* return the number of tracks */
   int tracks();
   /* return the first sector of a track or disc if no track is given */
   int firstSector( int track = -1 );
   /* return the last sector of a track or disc if no track is given */
   int lastSector( int track = -1 );
   /* return the length of a track or disc if no track is given if mm:ss format */
   QString length( int track = -1 );
   /* return if track is audio */
   bool isAudio( int track );
   /*  */
   QString title( int track = -1 );
   /*  */
   QString artist( int track = -1 );
   /*  */
   QString genre();
   /*  */
   int year();

private:
   CDInfo( const CDInfo &other );
   CDInfo &operator=( const CDInfo &other );

   /* Disc Info */
   int            mNumTracks;
   int            mDiscStartSector;
   int            mDiscEndSector;
   unsigned long  mDiscID;
   int            mDiscYear;
   QString        *mpDiscGenre;
   QString        *mpDiscArtist;
   QString        *mpDiscTitle;

   /* Tracks Info */
   int            *mpTrackStartSector;
   int            *mpTrackEndSector;
   bool           *mpTrackIsAudio;
   bool           *mpTrackCopyProhib;
   bool           *mpTrackPreEmp;
   bool           *mpTrack2Channels;
   QString        **mpTrackArtist;
   QString        **mpTrackTitle;
};

#endif
