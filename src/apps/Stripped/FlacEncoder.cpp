/**
 * src/apps/Stripped/FlacEncoder.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "FlacEncoder.hpp"

/* system headers */
#include <cstdio>

/* Qt headers */
#include <QtGui>

/* local library headers */
#include <MySettings.hpp>
#include <TagList.hpp>

/* local headers */
#include "ConfigDialog.hpp"


FlacEncoder::FlacEncoder( QWidget *parent )
: Encoder( parent, tr("FLAC") )
, mpConfigWidget( new QWidget( parent ) )
, mpQuality( new QSpinBox( mpConfigWidget ) )
, mpUseOga( new QCheckBox( tr("Use Ogg Container"), mpConfigWidget ) )
, mpEncoder( 0 )
, mpMetadata( 0 )
, mpPcm( 0 )
, mSize( 0 )
, mQuality( 0 )
, mUseOga( false )
{
   qsrand( time((time_t*)0) );

   QHBoxLayout *mainLayout = new QHBoxLayout( mpConfigWidget );
   mpQuality->setSingleStep( 1 );
   mpQuality->setMinimum( 0 );
   mpQuality->setMaximum( 8 );

#if QT_VERSION < 0x040300
   mainLayout->setMargin( 0 );
#else
   mainLayout->setContentsMargins( 0, 0, 0, 0 );
#endif
   mainLayout->addWidget( new QLabel( tr("Compression Level:"), mpConfigWidget ) );
   mainLayout->addWidget( mpQuality );
   mainLayout->addWidget( mpUseOga );

   mpConfigWidget->setLayout( mainLayout );

   readSettings();
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


void FlacEncoder::readSettings()
{
   MySettings settings;
   mQuality = settings.VALUE_FLACQUALITY;
   mUseOga  = settings.VALUE_FLACUSEOGA;
   mpQuality->setValue( mQuality );
   mpUseOga->setChecked( mUseOga );
}


void FlacEncoder::writeSettings()
{
   MySettings settings;
   mQuality = mpQuality->value();
   mUseOga  = mpUseOga->isChecked();
   settings.setValue( "FlacQuality", mQuality );
   settings.setValue( "FlacUseOga", mUseOga );
   delete mpEncoder;
   mpEncoder = 0;
}


bool FlacEncoder::initialize( const QString &fileName )
{
   bool                                     ok = true;
   FLAC__StreamMetadata                     *metadata[2];
   FLAC__StreamMetadata_VorbisComment_Entry entry;

   if( !mpEncoder )
   {
      mpEncoder = new FLAC::Encoder::File();
   }

   if( mUseOga )
   {
      if( !Encoder::initialize( fileName, ".oga" ) )
      {
         return false;
      }
      ok &= mpEncoder->set_ogg_serial_number( qrand() );
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

   metadata[0] = FLAC__metadata_object_new(FLAC__METADATA_TYPE_VORBIS_COMMENT);
   for( int i = 0; i < mTagList.count(); i++ )
   {
      if( !mTagList.valueAt(i).isEmpty() )
      {
         ok &= FLAC__metadata_object_vorbiscomment_entry_from_name_value_pair(
               &entry,
               mTagList.tagAt(i).toUtf8().constData(),
               mTagList.valueAt(i).toUtf8().constData() );
         ok &= FLAC__metadata_object_vorbiscomment_append_comment(
               metadata[0], entry, /*copy=*/false);
               /* copy=false: let metadata object take control of entry's allocated string */
      }
   }

   ok &= mpEncoder->set_metadata(metadata, 1);

   if( mUseOga )
   {
      /* not working yet, need to obtain ogg_serial */
      mpEncoder->set_ogg_serial_number( 0 );
      ok &= mpEncoder->init_ogg( ::fdopen( mFile.handle(), "w" ) );
   }
   else
   {
      ok &= mpEncoder->init( ::fdopen( mFile.handle(), "w" ) );
   }

   return ok;
}


bool FlacEncoder::finalize( bool enqueue, bool cancel )
{
   /* make sure all data is processed */
   wait();

   bool ok( true );
   ok &= mpEncoder->finish();
   ok &= Encoder::finalize( enqueue, cancel );
   return ok;
}


void FlacEncoder::encodeCDAudio( const QByteArray &data )
{
   if( !encodeCDAudio( data.constData(), data.size() ) )
   {
      emit encodingFail();
   }
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


QWidget *FlacEncoder::configWidget()
{
   return mpConfigWidget;
}
