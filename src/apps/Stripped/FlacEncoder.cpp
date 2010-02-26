/**
 * src/apps/Stripped/FlacEncoder.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#include "FlacEncoder.hpp"

#include "ConfigDialog.hpp"
#include "MySettings.hpp"
#include "TagList.hpp"

#include "Trace.hpp"

#include <stdio.h>

#include <QtGui>

FlacEncoder::FlacEncoder( QWidget *parent )
: Encoder( parent, tr("FLAC") )
, mUseOggContainer( false )
, mpEncoder( 0 )
, mpMetadata( 0 )
, mpPcm( 0 )
, mSize( 0 )
{
   QHBoxLayout *mainLayout = new QHBoxLayout( this );
   QSpinBox    *quality = new QSpinBox( this );
   quality->setValue( MySettings().VALUE_FLACQUALITY );
   quality->setSingleStep( 1 );
   quality->setMinimum( 0 );
   quality->setMaximum( 8 );

#if QT_VERSION < 0x040300
   mainLayout->setMargin( 0 );
#else
   mainLayout->setContentsMargins( 0, 0, 0, 0 );
#endif
   mainLayout->addWidget( quality );

   setLayout( mainLayout );
}


FlacEncoder::~FlacEncoder()
{
   if( mpEncoder )
   {
      delete mpEncoder;
      mpEncoder = 0;
   }
   if( mpMetadata )
   {
      delete mpMetadata;
      mpMetadata = 0;
   }
   if( mpPcm )
   {
      delete[] mpPcm;
      mpPcm = 0;
   }
}


void FlacEncoder::setQuality( int quality )
{
   MySettings().setValue( "FlacQuality", quality );
   mQuality = quality;
}


bool FlacEncoder::initialize( const QString &fileName )
{
   bool ok( true );

   if( !mpEncoder )
   {
      mpEncoder = new FLAC::Encoder::File();
   }

   if( mUseOggContainer )
   {
      if( !Encoder::initialize( fileName, ".oga" ) )
      {
         return false;
         long serial_number( 0 );
         /* TODO: find out how to determine serial number */
         mpEncoder->set_ogg_serial_number(serial_number);
      }
   }
   else
   {
      if( !Encoder::initialize( fileName, ".flac" ) )
      {
         return false;
      }
   }

   ok &= mpEncoder->set_verify(true);
   ok &= mpEncoder->set_compression_level(mQuality);
   ok &= mpEncoder->set_channels(2);
   ok &= mpEncoder->set_bits_per_sample(16);
   ok &= mpEncoder->set_sample_rate(44100);

   if( mUseOggContainer )
   {
      ok &= mpEncoder->init_ogg( ::fdopen( mFile.handle(), "w" ) );
   }
   else
   {
      ok &= mpEncoder->init( ::fdopen( mFile.handle(), "w" ) );
   }

   FLAC__StreamMetadata *metadata[1];
   FLAC__StreamMetadata_VorbisComment_Entry entry;

   for( int i = 0; i < mTagList.count(); i++ )
   {
      if( !mTagList.valueAt(i).isEmpty() )
      {
         metadata[0] = FLAC__metadata_object_new(FLAC__METADATA_TYPE_VORBIS_COMMENT);
         ok &= FLAC__metadata_object_vorbiscomment_entry_from_name_value_pair(
               &entry,
               mTagList.tagAt(i).toUtf8().constData(),
               mTagList.valueAt(i).toUtf8().constData() );
         ok &= FLAC__metadata_object_vorbiscomment_append_comment(
               metadata[0], entry, /*copy=*/false);
               /* copy=false: let metadata object take control of entry's allocated string */
      }
   }
#if 0
   metadata[1] = FLAC__metadata_object_new(FLAC__METADATA_TYPE_PADDING);
   metadata[1]->length = 1234; /* set the padding length */
#endif

   ok &= mpEncoder->set_metadata(metadata, 1);

   return ok;
}


bool FlacEncoder::finalize( bool enqueue, bool cancel )
{
   bool ok( true );
   ok &= mpEncoder->finish();
   ok &= Encoder::finalize( enqueue, cancel );
   return ok;
}


bool FlacEncoder::encodeCDAudio( const char* data, int size )
{
   bool ok( true );
   FLAC__byte  *buffer = (FLAC__byte*)data;
   if( size > mSize )
   {
      if( mpPcm )
      {
         delete[] mpPcm;
         mpPcm = 0;
      }
      mpPcm  = new FLAC__int32[size/2];
      mSize = size;
   }

   for( int i = 0; i < size/2; i++ )
   {
      /* inefficient but simple and works on big- or little-endian machines */
      mpPcm[i] = (FLAC__int32)(((FLAC__int16)(FLAC__int8)buffer[2*i+1] << 8) |
                 (FLAC__int16)buffer[2*i]);
   }
   /* feed samples to encoder */
   ok &= mpEncoder->process_interleaved(mpPcm, size/4);

   return ok;
}
