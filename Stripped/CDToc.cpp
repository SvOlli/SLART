/**
 * CDToc.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#include "CDToc.hpp"

#include "Trace.hpp"


CDToc::CDToc()
{
}


void CDToc::setDisc( int tracks, int startSector, int endSector )
{
   mNumTracks       = tracks;
   mStartSectorDisc = startSector;
   mEndSectorDisc   = endSector;
}


void CDToc::setEntry( int track, int startSector, int endSector,
                      bool isAudio, bool copyProhib, bool preEmp, bool twoChannels )
{
   mpStartSectorTrack[track] = startSector;
   mpEndSectorTrack[track]   = endSector;
   mpIsAudio[track]          = isAudio;
   mpCopyProhib[track]       = copyProhib;
   mpPreEmp[track]           = preEmp;
   mp2Channels[track]        = twoChannels;
   
   if( track == 1 )
   {
      setEntry( 0, mStartSectorDisc, startSector-1, isAudio, copyProhib, preEmp, twoChannels );
   }
}


int CDToc::firstSector( int track )
{
   if( track < 0 )
   {
      return mStartSectorDisc;
   }
   
   if( (track > mNumTracks) )
   {
      return 0;
   }
   else
   {
      return mpStartSectorTrack[track];
   }
}


int CDToc::lastSector( int track )
{
   if( track < 0 )
   {
      return mEndSectorDisc;
   }
   
   if( (track > mNumTracks) )
   {
      return 0;
   }
   else
   {
      return mpEndSectorTrack[track];
   }
}


int CDToc::cddbSum(int n)
{
   int ret = 0;
   
   while(n > 0)
   {
      ret += (n % 10);
      n /= 10;
   }
   
   return (ret);
}


void CDToc::calcCddbDiscID()
{
   int i = 0;
   int t = 0;
   int n = 0;

   for(i = 1; i <= mNumTracks; i++)
   {
      n += cddbSum( mpStartSectorTrack[i]/75 + 2 );
   }

   t = ((mEndSectorDisc-mStartSectorDisc)/75);

   mDiscID = ((n % 0xff) << 24 | t << 8 | mNumTracks);
}


QString CDToc::cddbDiscID()
{
   return QString::number( mDiscID, 16 );
}


void CDToc::dump()
{
TRACESTART(CDToc::dump)
TRACEMSG << "Tracks:" << mNumTracks 
         << "mStartSectorDisc:" << mStartSectorDisc
         << "mEndSectorDisc:" << mEndSectorDisc
         << "mDiscID:" << QString::number( mDiscID, 16 );
TRACEMSG << "#" << "mpStartSectorTrack" << "mpEndSectorTrack"
         << "mpIsAudio" << "mpCopyProhib" << "mpPreEmp" << "mp2Channels";
for( int i = 0; i <= mNumTracks; i++ )
{
TRACEMSG << i << mpStartSectorTrack[i] << mpEndSectorTrack[i]
         << mpIsAudio[i] << mpCopyProhib[i] << mpPreEmp[i] << mp2Channels[i]
         << mTitle[i];
}
TRACEMSG << query();
}


QString CDToc::length( int track )
{
   QString time;
   int msf, m, s, f;
   
   if( track < 0 )
   {
      msf = mEndSectorDisc - mStartSectorDisc;
   }
   else
   {
      msf = mpEndSectorTrack[track] - mpStartSectorTrack[track];
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


QString CDToc::query()
{
   QString querystring("cddb query ");
   querystring.append( QString::number( mDiscID, 16 ) );
   querystring.append( " " );
   querystring.append( QString::number( mNumTracks ) );
   for( int i = 1; i <= mNumTracks; i++ )
   {
      querystring.append( " " );
      querystring.append( QString::number( 150 + mpStartSectorTrack[i] ) );
   }
   querystring.append( " " );
   querystring.append( QString::number( (mEndSectorDisc-mStartSectorDisc) / 75 + 2 ) );
   
   return querystring;
}
