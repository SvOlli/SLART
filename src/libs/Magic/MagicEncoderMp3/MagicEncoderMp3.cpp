/*
 * src/apps/Magic/MagicEncoderMp3/MagicEncoderMp3.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

/* class declaration */
#include "MagicEncoderMp3.hpp"

/* system headers */
#include <cstdlib>
extern "C" {
#include <lame/lame.h>
}

/* Qt headers */
#include <QtGui>

/* local library headers */
#include <MySettings.hpp>
#include <ScrollLine.hpp>
#include <TagList.hpp>

/* local headers */

#include <Trace.hpp>

MagicEncoderMp3::MagicEncoderMp3()
: MagicEncoder( tr("mp3") )
, mUseAbr( false )
, mUseLatin1( false )
, mQuality( 4.0 )
, mpConfigWidget()
, mLame( 0 )
, mMp3BufferSize( 8192 )
, mpMp3Buffer( (unsigned char*)::malloc(mMp3BufferSize) )
{
}


MagicEncoderMp3::~MagicEncoderMp3()
{
   if( mpConfigWidget )
   {
      delete mpConfigWidget;
   }
   ::free( mpMp3Buffer );
   mpMp3Buffer = 0;
}


QByteArray MagicEncoderMp3::tagTo8Bit( int i )
{
   if( mUseLatin1 )
   {
      return mTagList.valueAt(i).toLatin1();
   }
   else
   {
      return mTagList.valueAt(i).toUtf8();
   }
}


bool MagicEncoderMp3::initialize( const QString &fileName )
{
   if( !MagicEncoder::initialize( fileName, ".mp3" ) )
   {
      return false;
   }

   mLame = ::lame_init();
   if( !mLame )
   {
      return false;
   }
   ::lame_set_in_samplerate( mLame, 44100 );
//   ::lame_set_brate( mLame, 128 );
//   ::lame_set_compression_ratio( mLame, 11.0 );
   ::lame_set_VBR( mLame, mUseAbr ? vbr_abr : vbr_default );
   ::lame_set_VBR_quality( mLame, mQuality );  /* 0..9 0:high 9:low */
   ::lame_init_params( mLame );

   id3tag_init( mLame );
   for( int i = 0; i < mTagList.count(); i++ )
   {
      if( !mTagList.valueAt(i).isEmpty() )
      {
         if( mTagList.tagAt(i) == "ALBUM" )
         {
            ::id3tag_set_album( mLame, tagTo8Bit(i).constData() );
         }
         else if( mTagList.tagAt(i) == "ARTIST" )
         {
            ::id3tag_set_artist( mLame, tagTo8Bit(i).constData() );
         }
         else if( mTagList.tagAt(i) == "TITLE" )
         {
            ::id3tag_set_title( mLame, tagTo8Bit(i).constData() );
         }
         else if( mTagList.tagAt(i) == "TRACKNUMBER" )
         {
            ::id3tag_set_track( mLame, tagTo8Bit(i).constData() );
         }
         else if( mTagList.tagAt(i) == "GENRE" )
         {
            ::id3tag_set_genre( mLame, tagTo8Bit(i).constData() );
         }
         else if( mTagList.tagAt(i) == "DATE" )
         {
            ::id3tag_set_year( mLame, tagTo8Bit(i).constData() );
         }
         else if( mTagList.tagAt(i) == "COMMENT" )
         {
            ::id3tag_set_comment( mLame, tagTo8Bit(i).constData() );
         }
      }
   }

   return true;
}


bool MagicEncoderMp3::finalize( bool enqueue, bool cancel )
{
   /* make sure all data is processed */
   wait();

   if( !encodeCDAudio( 0, 0 ) )
   {
      MagicEncoder::finalize( false, true );
      return false;
   }

   qint64 write = ::lame_encode_flush(mLame, mpMp3Buffer, 0);
   if( !MagicEncoder::writeChunk( (const char*)mpMp3Buffer, write ) )
   {
      MagicEncoder::finalize( false, true );
      return false;
   }
   lame_close( mLame );
   return MagicEncoder::finalize( enqueue, cancel );
}


void MagicEncoderMp3::encodeCDAudio( const QByteArray &data )
{
   if( !encodeCDAudio( data.constData(), data.size() ) )
   {
      emit encodingFail();
   }
}


bool MagicEncoderMp3::encodeCDAudio( const char* data, int size )
{
   short int *pcm = (short int*)data;
   int write = ::lame_encode_buffer_interleaved(mLame, pcm, size/4, mpMp3Buffer, mMp3BufferSize);
   if( !MagicEncoder::writeChunk( (const char*)mpMp3Buffer, (qint64)write ) )
   {
      MagicEncoder::finalize( false, true );
      return false;
   }
   return true;
}


MagicEncoderConfig *MagicEncoderMp3::configWidget( QWidget *parent, QAbstractButton *button )
{
   if( !mpConfigWidget )
   {
      mpConfigWidget = new MagicEncoderMp3Config( this, parent, button );
   }
   return mpConfigWidget;
}


Q_EXPORT_PLUGIN2(MagicEncoder, MagicEncoderMp3)
