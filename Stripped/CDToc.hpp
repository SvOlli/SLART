/**
 * CDToc.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#ifndef CDTOC_HPP
#define CDTOC_HPP CDTOC_HPP

#include <QString>

class CDToc
{
public:
   CDToc();

   /* set the global disc data (also resets all other data) */
   void setDisc( int tracks, int startSector, int endSector );
   /* set the data for a track */
   void setEntry( int track, int startSector, int endSector,
                  bool isAudio, bool copyProhib, bool preEmp, bool twoChannels );
   /* calculate the cddb discid */
   void calcCddbDiscID();
   /* return the cddb discid */
   QString cddbDiscID();
#if 0
   /* debug function: dump all data */
   void dump();
#endif
   /* return the first sector of a track or disc if no track is given */
   int firstSector( int track = -1 );
   /* return the last sector of a track or disc if no track is given */
   int lastSector( int track = -1 );
   /* return the length of a track or disc if no track is given if mm:ss format */
   QString length( int track = -1 );
   /* return if track is audio */
   bool isAudio( int track ) { return mpIsAudio[track]; };
   /* return the cddb query parameters */
   QString query();
   
   int          mYear;
   QString      mGenre;
   QString      mTitle[100];

private:
   CDToc( const CDToc &other );
   CDToc &operator=( const CDToc &other );

   /* subroutine for cddb discid calculation */
   int cddbSum( int i );

   int          mNumTracks;
   int          mStartSectorDisc;
   int          mEndSectorDisc;
   unsigned long mDiscID;
   int          mpStartSectorTrack[100];
   int          mpEndSectorTrack[100];
   bool         mpIsAudio[100];
   bool         mpCopyProhib[100];
   bool         mpPreEmp[100];
   bool         mp2Channels[100];
};

#endif
