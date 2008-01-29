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

   void setDisc( int tracks, int startSector, int endSector );
   void setEntry( int track, int startSector, int endSector,
                  bool isAudio, bool copyProhib, bool preEmp, bool twoChannels );
   void calcCddbDiscID();
   QString cddbDiscID();
   void dump();
   int firstSector( int track = -1 );
   int lastSector( int track = -1 );
   QString length( int track = -1 );
   QString query();

   int          mYear;
   QString      mGenre;
   QString      mTitle[100];

private:
   CDToc( const CDToc &other );
   CDToc &operator=( const CDToc &other );

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
