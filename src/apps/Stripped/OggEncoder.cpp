/**
 * src/apps/Stripped/OggEncoder.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#include "OggEncoder.hpp"
#include "MySettings.hpp"
#include "ConfigDialog.hpp"
#include "TagList.hpp"

#include "Trace.hpp"

extern "C"
{
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <math.h>
}

#include <QtGui>

OggEncoder::OggEncoder( QWidget *parent )
: Encoder( parent, tr("ogg") )
, mpQuality( new QDoubleSpinBox( this ) )
{
   QHBoxLayout *mainLayout = new QHBoxLayout( this );
   mpQuality->setSingleStep( 0.01 );
   mpQuality->setMinimum( 0.0 );
   mpQuality->setMaximum( 1.0 );
#if QT_VERSION < 0x040300
   mainLayout->setMargin( 0 );
#else
   mainLayout->setContentsMargins( 0, 0, 0, 0 );
#endif
   mainLayout->addWidget( new QLabel( tr("Quality:"), this ) );
   mainLayout->addWidget( mpQuality );

   setLayout( mainLayout );

   readSettings();
}


OggEncoder::~OggEncoder()
{
}


void OggEncoder::readSettings()
{
   mQuality = MySettings().VALUE_OGGQUALITY;
   mpQuality->setValue( mQuality );
}


void OggEncoder::writeSettings()
{
   mQuality = mpQuality->value();
   MySettings().setValue( "OggQuality", mQuality );
}


bool OggEncoder::initialize( const QString &fileName )
{
   if( !Encoder::initialize( fileName, ".ogg" ) )
   {
      return false;
   }
   mIsInit = false;
   vorbis_info_init( &mVorbisInfo );
   vorbis_comment_init( &mVorbisComment );

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


bool OggEncoder::finalize( bool enqueue, bool cancel )
{
   if( !encodeCDAudio( 0, 0 ) )
   {
      Encoder::finalize( false, true );
      return false;
   }
   
   ::ogg_stream_clear( &mOggPagegStream );
   ::vorbis_block_clear( &mVorbisBlock );
   ::vorbis_dsp_clear( &mVorbisDspState );
   ::vorbis_comment_clear( &mVorbisComment );
   ::vorbis_info_clear( &mVorbisInfo );
   
   return Encoder::finalize( enqueue, cancel );
}


bool OggEncoder::oggInit()
{
   int ret;
   
   ret = ::vorbis_encode_init_vbr( &mVorbisInfo, 2, 44100, mQuality );
   
   if( ret )
   {
      Encoder::finalize( false, true );
      return false;
   }
   
   vorbis_analysis_init( &mVorbisDspState, &mVorbisInfo );
   vorbis_block_init( &mVorbisDspState, &mVorbisBlock );

   srand(time(NULL));
   ogg_stream_init( &mOggPagegStream, rand() );
   
   ogg_packet header;
   ogg_packet header_comm;
   ogg_packet header_code;

   vorbis_analysis_headerout( &mVorbisDspState, &mVorbisComment, &header, &header_comm, &header_code );
   ogg_stream_packetin( &mOggPagegStream, &header );
   ogg_stream_packetin( &mOggPagegStream, &header_comm );
   ogg_stream_packetin( &mOggPagegStream, &header_code );
   
   for(;;)
   {
      if( !ogg_stream_flush( &mOggPagegStream, &mOggPage ) )
      {
         break;
      }
      if( !Encoder::writeChunk( (const char*)mOggPage.header, mOggPage.header_len ) )
      {
         Encoder::finalize( false, true );
         return false;
      }
      if( !Encoder::writeChunk( (const char*)mOggPage.body,   mOggPage.body_len ) )
      {
         Encoder::finalize( false, true );
         return false;
      }
   }

   return true;
}


bool OggEncoder::encodeCDAudio( const char* data, int size )
{
   if( !mIsInit )
   {
      mIsInit = true;
      if( !oggInit() )
      {
         Encoder::finalize( false, true );
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
            if( !Encoder::writeChunk( (const char*)mOggPage.header, mOggPage.header_len ) )
            {
               Encoder::finalize( false, true );
               return false;
            }
            if( !Encoder::writeChunk( (const char*)mOggPage.body,   mOggPage.body_len ) )
            {
               Encoder::finalize( false, true );
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
