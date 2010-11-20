/**
 * src/apps/Magic/MagicEncoderOgg/MagicEncoderOgg.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

/* class declaration */
#include "MagicEncoderOgg.hpp"

/* system headers */

/* Qt headers */
#include <QtGui>

/* local library headers */
#include <MySettings.hpp>
#include <ScrollLine.hpp>
#include <TagList.hpp>

/* local headers */


#include <Trace.hpp>

MagicEncoderOgg::MagicEncoderOgg()
: MagicEncoder( tr("ogg") )
, mQuality( 0.0 )
, mpConfigWidget()
, mIsInit( false )
{
}


MagicEncoderOgg::~MagicEncoderOgg()
{
   if( mpConfigWidget )
   {
      delete mpConfigWidget;
   }
}


bool MagicEncoderOgg::initialize( const QString &fileName )
{
   if( !MagicEncoder::initialize( fileName, ".ogg" ) )
   {
      return false;
   }
   mIsInit = false;
   ::vorbis_info_init( &mVorbisInfo );
   ::vorbis_comment_init( &mVorbisComment );

   for( int i = 0; i < mTagList.count(); i++ )
   {
      if( !mTagList.valueAt(i).isEmpty() )
      {
         ::vorbis_comment_add_tag( &mVorbisComment,
                                    mTagList.tagAt(i).toUtf8().data(),
                                    mTagList.valueAt(i).toUtf8().data() );
      }
   }
   return true;
}


bool MagicEncoderOgg::finalize( bool enqueue, bool cancel )
{
   /* make sure all data is processed */
   wait();

   if( !encodeCDAudio( 0, 0 ) )
   {
      MagicEncoder::finalize( false, true );
      return false;
   }

   ::ogg_stream_clear( &mOggPagegStream );
   ::vorbis_block_clear( &mVorbisBlock );
   ::vorbis_dsp_clear( &mVorbisDspState );
   ::vorbis_comment_clear( &mVorbisComment );
   ::vorbis_info_clear( &mVorbisInfo );

   return MagicEncoder::finalize( enqueue, cancel );
}


bool MagicEncoderOgg::oggInit()
{
   int ret;

   ret = ::vorbis_encode_init_vbr( &mVorbisInfo, 2, 44100, mQuality );

   if( ret )
   {
      MagicEncoder::finalize( false, true );
      return false;
   }

   ::vorbis_analysis_init( &mVorbisDspState, &mVorbisInfo );
   ::vorbis_block_init( &mVorbisDspState, &mVorbisBlock );

   srand(time(NULL));
   ::ogg_stream_init( &mOggPagegStream, rand() );

   ogg_packet header;
   ogg_packet header_comm;
   ogg_packet header_code;

   ::vorbis_analysis_headerout( &mVorbisDspState, &mVorbisComment, &header, &header_comm, &header_code );
   ::ogg_stream_packetin( &mOggPagegStream, &header );
   ::ogg_stream_packetin( &mOggPagegStream, &header_comm );
   ::ogg_stream_packetin( &mOggPagegStream, &header_code );

   for(;;)
   {
      if( !::ogg_stream_flush( &mOggPagegStream, &mOggPage ) )
      {
         break;
      }
      if( !MagicEncoder::writeChunk( (const char*)mOggPage.header, mOggPage.header_len ) )
      {
         MagicEncoder::finalize( false, true );
         return false;
      }
      if( !MagicEncoder::writeChunk( (const char*)mOggPage.body,   mOggPage.body_len ) )
      {
         MagicEncoder::finalize( false, true );
         return false;
      }
   }

   return true;
}


void MagicEncoderOgg::encodeCDAudio( const QByteArray &data )
{
   if( !encodeCDAudio( data.constData(), data.size() ) )
   {
      emit encodingFail();
   }
}


bool MagicEncoderOgg::encodeCDAudio( const char* data, int size )
{
   if( !mIsInit )
   {
      mIsInit = true;
      if( !oggInit() )
      {
         MagicEncoder::finalize( false, true );
         return false;
      }
   }

   int i = 0;
   bool eos = false;
   float **buffer = ::vorbis_analysis_buffer( &mVorbisDspState, size/4 );

   for( i = 0; i < size/4; i++ )
   {
      buffer[0][i]=((0x00ff&(int)data[i*4  ])|(data[i*4+1]<<8))/32768.f;
      buffer[1][i]=((0x00ff&(int)data[i*4+2])|(data[i*4+3]<<8))/32768.f;
   }
   ::vorbis_analysis_wrote( &mVorbisDspState, i );

   while( ::vorbis_analysis_blockout( &mVorbisDspState, &mVorbisBlock ) == 1 )
   {
      ::vorbis_analysis( &mVorbisBlock, 0 );
      ::vorbis_bitrate_addblock( &mVorbisBlock );

      while( ::vorbis_bitrate_flushpacket( &mVorbisDspState, &mOggPacket ) )
      {
         ::ogg_stream_packetin( &mOggPagegStream, &mOggPacket );

         while( !eos )
         {
            if( !::ogg_stream_pageout( &mOggPagegStream, &mOggPage ) )
            {
               break;
            }
            if( !MagicEncoder::writeChunk( (const char*)mOggPage.header, mOggPage.header_len ) )
            {
               MagicEncoder::finalize( false, true );
               return false;
            }
            if( !MagicEncoder::writeChunk( (const char*)mOggPage.body,   mOggPage.body_len ) )
            {
               MagicEncoder::finalize( false, true );
               return false;
            }
         }

         if( ::ogg_page_eos( &mOggPage ) )
         {
            eos = true;
         }
      }
   }
   return true;
}


MagicEncoderConfig *MagicEncoderOgg::configWidget( QWidget *parent, QAbstractButton *button )
{
   if( !mpConfigWidget )
   {
      mpConfigWidget = new MagicEncoderOggConfig( this, parent, button );
   }
   return mpConfigWidget;
}


Q_EXPORT_PLUGIN2(MagicEncoder, MagicEncoderOgg)
