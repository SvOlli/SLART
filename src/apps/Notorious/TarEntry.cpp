/**
 * src/apps/Notorious/TarEntry.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "TarEntry.hpp"

/* system headers */

/* Qt headers */

/* local library headers */

/* local headers */


#include <Trace.hpp>

TarEntry::TarEntry()
: mDirty( false )
#if INCLUDE_EXT
, mSQLLine( "REPLACE INTO freedb_%1(id,track,title,playtime,ext)"
            "VALUES('%2',%3,'%4',%5,'%6');" )
#else
, mSQLLine( "REPLACE INTO freedb_%1(id,track,title,playtime)"
            "VALUES('%2',%3,'%4',%5);" )
#endif
, mCR( '\r' )
, mLF( '\n' )
, mBadData( 0xFFFD )
, mQuote( '\'' )
, mDoubleQuote( "\'\'" )
{
}


TarEntry::~TarEntry()
{
}


void TarEntry::setData( const char *filename, const char *data )
{
#if 0
TRACESTART(TarEntry::setData)
#endif
   int slashpos;
   const char *p;

   mCategory = filename;
   slashpos  = mCategory.indexOf('/');
   if( slashpos < 1 )
   {
      mSQL.clear();
      mDirty = false;
      mTracks = -1;
      return;
   }
   p         = filename + slashpos + 1;
   mID       = p;
   mCategory = mCategory.left( slashpos );
   // add some magical "guess what 8bit encoding it is"-code
   mData     = QString::fromUtf8(data);
   if( mData.contains( mBadData ) )
   {
      mData     = QString::fromLatin1(data);
   }

   mData.remove( mCR );
   mLines = mData.split( mLF );

   mSQL.clear();

   mCategory.replace( mQuote, mDoubleQuote );
   mID.replace( mQuote, mDoubleQuote );
   mDirty    = true;
}


QStringList TarEntry::sql()
{
#if 0
TRACESTART(TarEntry::sql)
#endif
   const QRegExp reLength("^# Disc length:[ \t]*");
   const QRegExp reAfterSpace( " .*" );
#if INCLUDE_EXT
   const QRegExp reExtD("^EXTD=");
   const QRegExp reExtT("^EXTT[0-9]*=");
#endif
   const QRegExp reDTitle("^DTITLE=");
   const QRegExp reTTitle("^TTITLE[0-9]*=");

   QString line;
   bool ok;

   if( mDirty )
   {
      int  timescan = 0;
      mStartframe[0] = 0;

      QString line;
      foreach( line, mLines )
      {
         if( line.contains( reLength ) )
         {
            line.remove( reLength );
            line.remove( reAfterSpace );
            int seconds = line.toInt( &ok );
            mStartframe[timescan] = 75 * seconds;
            mStartframe[0] = mStartframe[timescan];
            mTracks = timescan-1;
            timescan = 0;
            for( int i = 0; i <= mTracks; i++ )
            {
#if INCLUDE_EXT
               mExt[i].clear();
#endif
               mTitle[i].clear();
            }
         }

         if( timescan )
         {
            line.remove( QRegExp( "^#[ \t]*" ) );
            int frames = line.toInt( &ok );
            if( ok )
            {
               mStartframe[timescan] = frames;
               timescan++;
            }
         }
         else
         {
            if( line.contains( reTTitle ) )
            {
               int tracknr = line.mid(6,2).toInt( &ok );
               if( !ok )
               {
                  tracknr = line.mid(6,1).toInt( &ok );
               }
               line.remove( reTTitle );
               mTitle[1+tracknr].append( line );
            }
#if INCLUDE_EXT
            else if( line.contains( reExtT ) )
            {
               int tracknr = line.mid(4,2).toInt( &ok );
               if( !ok )
               {
                  tracknr = line.mid(4,1).toInt( &ok );
               }
               line.remove( reExtT );
               mExt[1+tracknr].append( line );
            }
#endif
            else if( line.contains( reDTitle ) )
            {
               line.remove( reDTitle );
               mTitle[0].append( line );
            }
#if INCLUDE_EXT
            else if( line.contains( reExtD ) )
            {
               line.remove( reExtD );
               mExt[0].append( line );
            }
#endif
         }

         if( line.startsWith( "# Track frame offsets:" ) )
         {
            timescan = 1;
         }
      }

      for( int i = 1; i < mTracks; i++ )
      {
         mPlaytime[i] = mStartframe[i+1] - mStartframe[i];
      }
      mPlaytime[mTracks] = mStartframe[0] - mStartframe[mTracks];
      mPlaytime[0] = mStartframe[0];
      for( int i = 0; i <= mTracks; i++ )
      {
         mTitle[i].replace( mQuote, mDoubleQuote );
#if INCLUDE_EXT
         mExt[i].replace( mQuote, mDoubleQuote );
#endif
      }
   }
#if 0
TRACEMSG << mTracks;
   for( int linenr = 0; linenr <= mTracks; linenr++ )
   {
TRACEMSG << linenr << mStartframe[linenr] << mPlaytime[linenr]/75 << mTitle[linenr] << mExt[linenr];
   }
#endif

   mSQL.clear();
   for( int i = 0; i <= mTracks; i++ )
   {
#if INCLUDE_EXT
      mSQL.append( mSQLLine.arg(mCategory,mID,QString::number(i),mTitle[i],QString::number(mPlaytime[i]),mExt[i]) );
#else
      mSQL.append( mSQLLine.arg(mCategory,mID,QString::number(i),mTitle[i],QString::number(mPlaytime[i])) );
#endif
   }
   return mSQL;
}
