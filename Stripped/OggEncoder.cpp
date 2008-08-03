/**
 * OggEncoder.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#include "OggEncoder.hpp"
#include "MySettings.hpp"
#include "TagList.hpp"

#include "Trace.hpp"

extern "C"
{
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
}

#include <QtGui>

OggEncoder::OggEncoder( QWidget *parent )
: Encoder( parent, tr("ogg") )
{
   MySettings settings;
   QHBoxLayout *mainLayout = new QHBoxLayout( this );
   QDoubleSpinBox *quality = new QDoubleSpinBox( this );
   quality->setValue( settings.value("OggQuality", 0.4).toDouble() );
   quality->setSingleStep( 0.01 );
   quality->setMinimum( 0.0 );
   quality->setMaximum( 1.0 );
#if QT_VERSION < 0x040300
   mainLayout->setMargin( 0 );
#else
   mainLayout->setContentsMargins( 0, 0, 0, 0 );
#endif
   mainLayout->addWidget( quality );

   connect( quality, SIGNAL(valueChanged(double)), 
            this, SLOT(setQuality(double)) );

   setLayout( mainLayout );
}


OggEncoder::~OggEncoder()
{
}


void OggEncoder::setQuality( double quality )
{
   MySettings settings;
   settings.setValue( "OggQuality", quality );
   mQuality = quality;
}


void OggEncoder::initialize( const QString &fileName )
{
   Encoder::initialize( fileName, ".ogg" );
   mIsInit = false;
   mQuality = 0.4;
   vorbis_info_init( &mVI );
   vorbis_comment_init( &mVC );
}


void OggEncoder::finalize( bool enqueue, bool cancel )
{
   encodeCDAudio( 0, 0 );
   
   ::ogg_stream_clear( &mOS );
   ::vorbis_block_clear( &mVB );
   ::vorbis_dsp_clear( &mVD );
   ::vorbis_comment_clear( &mVC );
   ::vorbis_info_clear( &mVI );
   
   Encoder::finalize( enqueue, cancel );
}


bool OggEncoder::oggInit()
{
   int ret;
   
   ret = ::vorbis_encode_init_vbr( &mVI, 2, 44100, mQuality );
   
   if( ret )
   {
      exit(1);
   }
   
   vorbis_analysis_init( &mVD, &mVI );
   vorbis_block_init( &mVD, &mVB );

   srand(time(NULL));
   ogg_stream_init( &mOS, rand() );
   
   ogg_packet header;
   ogg_packet header_comm;
   ogg_packet header_code;

   vorbis_analysis_headerout( &mVD, &mVC, &header, &header_comm, &header_code );
   ogg_stream_packetin( &mOS, &header );
   ogg_stream_packetin( &mOS, &header_comm );
   ogg_stream_packetin( &mOS, &header_code );
   
   for(;;)
   {
      if( 0 == ogg_stream_flush( &mOS, &mOG ) )
      {
         break;
      }
      if( ::write( mFD, mOG.header, mOG.header_len ) < 0 )
      {
         return false;
      }
      if( ::write( mFD, mOG.body,   mOG.body_len ) < 0 )
      {
         return false;
      }
   }
   return true;
}


void OggEncoder::setTags( const TagList &tagList )
{
   for( int i = 0; i < tagList.count(); i++ )
   {
      if( !tagList.valueAt(i).isEmpty() )
      {
         ::vorbis_comment_add_tag( &mVC, 
                                   tagList.tagAt(i).toUtf8().data(),
                                   tagList.valueAt(i).toUtf8().data() );
      }
   }
}


bool OggEncoder::encodeCDAudio( const char* data, int size )
{
   if( !mIsInit )
   {
      mIsInit = true;
      if( !oggInit() )
      {
         return false;
      }
   }
   
   int i;
   bool eos = false;
   float **buffer = ::vorbis_analysis_buffer( &mVD, size/4 );
   
   for( i = 0; i < size/4; i++ )
   {
      buffer[0][i]=((0x00ff&(int)data[i*4  ])|(data[i*4+1]<<8))/32768.f;
      buffer[1][i]=((0x00ff&(int)data[i*4+2])|(data[i*4+3]<<8))/32768.f;
   }
   ::vorbis_analysis_wrote( &mVD, i );
   
   while( ::vorbis_analysis_blockout( &mVD, &mVB ) == 1 )
   {
      ::vorbis_analysis( &mVB, 0 );
      ::vorbis_bitrate_addblock( &mVB );
      
      while( ::vorbis_bitrate_flushpacket( &mVD, &mOP ) )
      {
         ::ogg_stream_packetin( &mOS, &mOP );
         
         while( !eos )
         {
            i = ::ogg_stream_pageout( &mOS, &mOG );
            if( i == 0 ) break;
            
            if( ::write( mFD, mOG.header, mOG.header_len ) < 0 )
            {
               return false;
            }
            if( ::write( mFD, mOG.body,   mOG.body_len ) < 0 )
            {
               return false;
            }
         }
         
         if( ::ogg_page_eos( &mOG ) ) eos = true;
      }
   }
   return true;
}
