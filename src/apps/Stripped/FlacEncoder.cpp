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
#include <ScrollLine.hpp>
#include <TagList.hpp>

/* local headers */
#include "ConfigDialog.hpp"


FlacEncoder::FlacEncoder( QWidget *parent )
: Encoder( parent, tr("FLAC") )
, mQuality( 0 )
, mUseOga( false )
, mpConfigWidget( new QWidget( parent ) )
, mpUseEncoder( new QCheckBox( tr("Use This Encoder"), mpConfigWidget ) )
, mpDirOverride( new QCheckBox( tr("Override Base Directory"), mpConfigWidget ) )
, mpDirectory( new ScrollLine( mpConfigWidget ) )
, mpDotButton( new QPushButton( tr("..."), mpConfigWidget ) )
, mpQuality( new QSpinBox( mpConfigWidget ) )
, mpUseOga( new QCheckBox( tr("Use Ogg Container"), mpConfigWidget ) )
, mpEncoder( 0 )
, mpMetadata( 0 )
, mpPcm( 0 )
, mSize( 0 )
{
   qsrand( time((time_t*)0) );

   mpQuality->setSingleStep( 1 );
   mpQuality->setMinimum( 0 );
   mpQuality->setMaximum( 8 );
   mpQuality->setToolTip( tr("0 = fastest; 8 = best") );

   QGridLayout *mainLayout = new QGridLayout( mpConfigWidget );
   mainLayout->setRowStretch( 5, 1 );
   mainLayout->setColumnStretch( 1, 1 );
#if QT_VERSION < 0x040300
   mainLayout->setMargin( 3 );
#else
   mainLayout->setContentsMargins( 3, 3, 3, 3 );
#endif
   mainLayout->addWidget( mpUseEncoder, 0, 0, 1, 3 );
   mainLayout->addWidget( mpDirOverride, 1, 0, 1, 3 );
   mainLayout->addWidget( new QLabel( tr("Base Directory:") ), 2, 0 );
   mainLayout->addWidget( mpDirectory, 2, 1 );
   mainLayout->addWidget( mpDotButton, 2, 2 );
   mainLayout->addWidget( new QLabel( tr("Compression Level:"), mpConfigWidget ), 3, 0 );
   mainLayout->addWidget( mpQuality, 3, 1, 1, 2 );
   mainLayout->addWidget( mpUseOga,  4, 0 );

   mpConfigWidget->setLayout( mainLayout );

   connect( mpUseEncoder, SIGNAL(clicked(bool)),
            this, SIGNAL(useEncoderClicked(bool)) );
   connect( mpDotButton, SIGNAL(clicked()),
            this, SLOT(handleDotButton()) );

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


void FlacEncoder::setUseEncoder( bool on )
{
   mpUseEncoder->setChecked( on );
}


void FlacEncoder::readSettings()
{
   MySettings settings;
   settings.beginGroup( mName );
   mUseEncoder  = settings.VALUE_USE_ENCODER;
   mDirOverride = settings.VALUE_DIRECTORY_OVERRIDE;
   mDirectory   = settings.VALUE_DIRECTORY;
   mQuality     = settings.VALUE_FLACQUALITY;
   mUseOga      = settings.VALUE_FLACUSEOGA;
   mpUseEncoder->setChecked( mUseEncoder );
   mpDirOverride->setChecked( mDirOverride );
   mpDirectory->setText( mDirectory );
   mpQuality->setValue( mQuality );
   mpUseOga->setChecked( mUseOga );
   settings.endGroup();
}


void FlacEncoder::writeSettings()
{
   MySettings settings;
   settings.beginGroup( mName );
   mUseEncoder  = mpUseEncoder->isChecked();
   mDirOverride = mpDirOverride->isChecked();
   mDirectory   = mpDirectory->text();
   mQuality     = mpQuality->value();
   mUseOga      = mpUseOga->isChecked();
   settings.setValue( "UseEncoder", mUseEncoder );
   settings.setValue( "DirectoryOverride", mDirOverride );
   settings.setValue( "Directory", mDirectory );
   settings.setValue( "FlacQuality", mQuality );
   settings.setValue( "FlacUseOga", mUseOga );
   settings.endGroup();
   if( mpEncoder )
   {
      delete mpEncoder;
      mpEncoder = 0;
   }
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


void FlacEncoder::handleDotButton()
{
   Encoder::setDirectory( mpDirectory );
}
