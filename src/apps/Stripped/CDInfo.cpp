/*
 * src/apps/Stripped/CDInfo.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "CDInfo.hpp"

/* system headers */

/* Qt headers */

/* local library headers */

/* local headers */


#define SAVEDELETE(x) if( x ) { delete x; x = 0; }
#define SAVEDELETEARRAY(x) if( x ) { delete[] x; x = 0; }



CDInfo::CDInfo()
: mNumTracks( 0 )
, mDiscStartSector( 0 )
, mDiscEndSector( 0 )
, mDiscID( 0 )
, mDiscYear( 0 )
, mpDiscGenre( new QString() )
, mpDiscArtist( new QString() )
, mpDiscTitle( new QString() )
, mpTrackStartSector( new int[100] )
, mpTrackEndSector( new int[100] )
, mpTrackIsAudio( new bool[100] )
, mpTrackCopyProhib( new bool[100] )
, mpTrackPreEmp( new bool[100] )
, mpTrack2Channels( new bool[100] )
, mpTrackArtist( new QString*[100] )
, mpTrackTitle( new QString*[100] )
{
   for( int i = 0; i < 100; i++ )
   {
      mpTrackArtist[i] = new QString();
      mpTrackTitle[i]  = new QString();
   }
}


CDInfo::~CDInfo()
{
   SAVEDELETE( mpDiscGenre );
   SAVEDELETE( mpDiscArtist );
   SAVEDELETE( mpDiscTitle );

   SAVEDELETEARRAY( mpTrackStartSector );
   SAVEDELETEARRAY( mpTrackEndSector );
   SAVEDELETEARRAY( mpTrackIsAudio );
   SAVEDELETEARRAY( mpTrackCopyProhib );
   SAVEDELETEARRAY( mpTrackPreEmp );
   SAVEDELETEARRAY( mpTrack2Channels );
   for( int i = 0; i < 100; i++ )
   {
      SAVEDELETE( mpTrackArtist[i] );
      SAVEDELETE( mpTrackTitle[i] );
   }
   SAVEDELETEARRAY( mpTrackArtist );
   SAVEDELETEARRAY( mpTrackTitle );
}


void CDInfo::clear()
{
   mNumTracks = 0;
   mDiscStartSector = 0;
   mDiscEndSector = 0;
   mDiscID = 0;
   mDiscYear = 0;
   mpDiscGenre->clear();
   mpDiscArtist->clear();
   mpDiscTitle->clear();

   for( int i = 0; i < 100; i ++ )
   {
      mpTrackStartSector[i] = 0;
      mpTrackEndSector[i]   = 0;
      mpTrackIsAudio[i]     = false;
      mpTrackCopyProhib[i]  = false;
      mpTrackPreEmp[i]      = false;
      mpTrack2Channels[i]   = false;
      mpTrackArtist[i]->clear();
      mpTrackTitle[i]->clear();
   }
}


void CDInfo::setDisc( int tracks, int startSector, int endSector )
{
   mNumTracks       = tracks;
   mDiscStartSector = startSector;
   mDiscEndSector   = endSector;
}


void CDInfo::setTrack( int track, int startSector, int endSector,
                       bool isAudio, bool copyProhib, bool preEmp, bool twoChannels )
{
   mpTrackStartSector[track] = startSector;
   mpTrackEndSector[track]   = endSector;
   mpTrackIsAudio[track]     = isAudio;
   mpTrackCopyProhib[track]  = copyProhib;
   mpTrackPreEmp[track]      = preEmp;
   mpTrack2Channels[track]   = twoChannels;

   if( track == 1 )
   {
      /* set the hidden track before the first one */
      setTrack( 0, mDiscStartSector, startSector-1, isAudio, copyProhib, preEmp, twoChannels );
   }
}


void CDInfo::setArtist( int track, const QString &artist, bool append )
{
   if( track < 0 )
   {
      if( !append )
      {
         mpDiscArtist->clear();
      }
      mpDiscArtist->append( artist );
   }
   else if( track < 100 )
   {
      if( !append )
      {
         mpTrackArtist[track]->clear();
      }
      mpTrackArtist[track]->append( artist );
   }
}


void CDInfo::setTitle( int track, const QString &title, bool append )
{
   if( track < 0 )
   {
      if( !append )
      {
         mpDiscTitle->clear();
      }
      mpDiscTitle->append( title );
   }
   else if( track < 100 )
   {
      if( !append )
      {
         mpTrackTitle[track]->clear();
      }
      mpTrackTitle[track]->append( title );
   }
}


void CDInfo::setGenre( const QString &genre, bool append )
{
   if( !append )
   {
      mpDiscGenre->clear();
   }
   mpDiscGenre->append( genre );
}


void CDInfo::setYear( int year )
{
   mDiscYear = year;
}


void CDInfo::calcCddbDiscID()
{
   int i = 0;
   int t = 0;
   int n = 0;
   int m = 0;

   for(i = 1; i <= mNumTracks; i++)
   {
      m = mpTrackStartSector[i]/75 + 2;
      t = 0;
      while( m > 0 )
      {
         t += (m % 10);
         m /= 10;
      }

      n += t;
   }
   n = n % 0xff;

   t = ((mDiscEndSector + 1) - mDiscStartSector) / 75;

   mDiscID = (n << 24 | t << 8 | mNumTracks);
}


QString CDInfo::cddbDiscID()
{
   return QString::number( mDiscID, 16 ).right(8).rightJustified(8,'0');
}


int CDInfo::tracks()
{
   return mNumTracks;
}


int CDInfo::firstSector( int track )
{
   if( track < 0 )
   {
      return mDiscStartSector;
   }

   if( (track > mNumTracks) )
   {
      return 0;
   }
   else
   {
      return mpTrackStartSector[track];
   }
}


int CDInfo::lastSector( int track )
{
   if( track < 0 )
   {
      return mDiscEndSector;
   }

   if( (track > mNumTracks) )
   {
      return 0;
   }
   else
   {
      return mpTrackEndSector[track];
   }
}


QString CDInfo::length( int track )
{
   QString time;
   int msf = 0;
   int m   = 0;
   int s   = 0;
   int f   = 0;

   if( track < 0 )
   {
      msf = mDiscEndSector - mDiscStartSector;
   }
   else if( track < 100 )
   {
      msf = mpTrackEndSector[track] - mpTrackStartSector[track];
   }

   if( msf > 0 )
   {
      char msftext[] = "    0.00";
      m = msf / 4500;
      s = (msf / 75) % 60;
      f = msf % 75;

      if( m > 9 )
      {
         msftext[0] = '0'+(m/10);
      }
      if( m > 0 )
      {
         msftext[1] = '0'+(m%10);
         msftext[2] = ':';
         msftext[3] = '0'+(s/10);
      }
      else
      {
         if( s > 10 )
         {
            msftext[3] = '0'+(s/10);
         }
      }
      msftext[4] = '0'+(s%10);
      msftext[6] = '0'+(f/10);
      msftext[7] = '0'+(f%10);

      time = msftext;
   }
   return time;
}


bool CDInfo::isAudio( int track )
{
   return mpTrackIsAudio[track];
}


QString CDInfo::artist( int track )
{
   if( track < 0 )
   {
      return *mpDiscArtist;
   }
   else if( track < 100 )
   {
      return *mpTrackArtist[track];
   }
   else
   {
      return QString();
   }
}


QString CDInfo::title( int track )
{
   if( track < 0 )
   {
      return *mpDiscTitle;
   }
   else if( track < 100 )
   {
      return *mpTrackTitle[track];
   }
   else
   {
      return QString();
   }
}


QString CDInfo::genre()
{
   return *mpDiscGenre;
}

int CDInfo::year()
{
   return mDiscYear;
}


void CDInfo::splitTitle( const QString &separator )
{
   QString title( *mpDiscTitle );
   int i;
   int indexOfSplit = title.indexOf( separator );

   if( indexOfSplit > 0 )
   {
      *mpDiscArtist = title.left( indexOfSplit );
      *mpDiscTitle  = title.mid( indexOfSplit + separator.size() );
   }

   for( i = 0; i < 100; i++ )
   {
      title = *mpTrackTitle[i];
      indexOfSplit = title.indexOf( separator );

      if( indexOfSplit > 0 )
      {
         *mpTrackArtist[i] = title.left( indexOfSplit );
         *mpTrackTitle[i]  = title.mid( indexOfSplit + separator.size() );
      }
      else
      {
         *mpTrackArtist[i] = *mpDiscArtist;
      }
   }
}
