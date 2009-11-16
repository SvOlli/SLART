
#include "TarEntry.hpp"
#include "Trace.hpp"


TarEntry::TarEntry()
: mDirty( false )
, mCR( '\r' )
, mLF( '\n' )
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
   p         = filename + slashpos + 1;
   mID       = p;
   mCategory = mCategory.left( slashpos );
   mData     = data;
   
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
   const QRegExp reExtD("^EXTD=");
   const QRegExp reExtT("^EXTT[0-9]*=");
   const QRegExp reDTitle("^DTITLE=");
   const QRegExp reTTitle("^TTITLE[0-9]*=");
   
   QString line;
   bool ok;
   
   if( mDirty )
   {
      int  timescan = 0;
      mStartframe[0] = 0;
      
      for( int linenr = 0; linenr < mLines.size(); linenr++ )
      {
         line = mLines.at(linenr);
         
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
               mExt[i].clear();
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
            else if( line.contains( reDTitle ) )
            {
               line.remove( reDTitle );
               mTitle[0].append( line );
            }
            else if( line.contains( reExtD ) )
            {
               line.remove( reExtD );
               mExt[0].append( line );
            }
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
         mExt[i].replace( mQuote, mDoubleQuote );
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
      mSQLLine.clear();
      mSQLLine.append( "REPLACE INTO freedb (category,id,track,title,playtime,ext) VALUES('" );
      mSQLLine.append( mCategory );
      mSQLLine.append( "','" );
      mSQLLine.append( mID );
      mSQLLine.append( "'," );
      mSQLLine.append( QString::number( i ) );
      mSQLLine.append( ",'" );
      mSQLLine.append( mTitle[i] );
      mSQLLine.append( "'," );
      mSQLLine.append( QString::number( mPlaytime[i] ) );
      mSQLLine.append( ",'" );
      mSQLLine.append( mExt[i] );
      mSQLLine.append( "');" );
      mSQL.append( mSQLLine );
   }
   return mSQL;
}
