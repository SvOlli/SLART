/**
 * src/apps/Stripped/OggEncoder.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "OggEncoder.hpp"

/* system headers */

/* Qt headers */
#include <QtGui>

/* local library headers */
#include <MySettings.hpp>
#include <ScrollLine.hpp>
#include <TagList.hpp>

/* local headers */
#include "ConfigDialog.hpp"


#include <Trace.hpp>

OggEncoder::OggEncoder( QWidget *parent )
: Encoder( parent, tr("ogg") )
, mQuality( 0.0 )
, mpConfigWidget( new QWidget( parent ) )
, mpUseEncoder( new QCheckBox( tr("Use This Encoder"), mpConfigWidget ) )
, mpDirOverride( new QCheckBox( tr("Override Base Directory"), mpConfigWidget ) )
, mpDirectory( new ScrollLine( mpConfigWidget ) )
, mpDotButton( new QPushButton( tr("..."), mpConfigWidget ) )
, mpQuality( new QDoubleSpinBox( mpConfigWidget ) )
{
   mpQuality->setSingleStep( 0.01 );
   mpQuality->setMinimum( 0.0 );
   mpQuality->setMaximum( 1.0 );
   mpQuality->setToolTip( tr("0 = low & short, 1 = high & long") );

   QGridLayout *mainLayout = new QGridLayout( mpConfigWidget );
   mainLayout->setRowStretch( 4, 1 );
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
   mainLayout->addWidget( new QLabel( tr("Quality:"), mpConfigWidget ), 3, 0 );
   mainLayout->addWidget( mpQuality, 3, 1, 1, 2 );

   mpConfigWidget->setLayout( mainLayout );

   connect( mpUseEncoder, SIGNAL(clicked(bool)),
            this, SIGNAL(useEncoderClicked(bool)) );
   connect( mpDotButton, SIGNAL(clicked()),
            this, SLOT(handleDotButton()) );

   readSettings();
}


OggEncoder::~OggEncoder()
{
}


void OggEncoder::setUseEncoder( bool on )
{
   mpUseEncoder->setChecked( on );
}


void OggEncoder::readSettings()
{
   MySettings settings;
   settings.beginGroup( mName );
   mUseEncoder  = settings.VALUE_USE_ENCODER;
   mDirOverride = settings.VALUE_DIRECTORY_OVERRIDE;
   mDirectory   = settings.VALUE_DIRECTORY;
   mQuality     = settings.VALUE_OGGQUALITY;
   mpUseEncoder->setChecked( mUseEncoder );
   mpDirOverride->setChecked( mDirOverride );
   mpDirectory->setText( mDirectory );
   mpQuality->setValue( mQuality );
   settings.endGroup();
}


void OggEncoder::writeSettings()
{
   MySettings settings;
   settings.beginGroup( mName );
   mUseEncoder  = mpUseEncoder->isChecked();
   mDirOverride = mpDirOverride->isChecked();
   mDirectory   = mpDirectory->text();
   mQuality     = mpQuality->value();
   settings.setValue( "UseEncoder", mUseEncoder );
   settings.setValue( "DirectoryOverride", mDirOverride );
   settings.setValue( "Directory", mDirectory );
   settings.setValue( "OggQuality", mQuality );
   settings.endGroup();
}


bool OggEncoder::initialize( const QString &fileName )
{
   if( !Encoder::initialize( fileName, ".ogg" ) )
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


bool OggEncoder::finalize( bool enqueue, bool cancel )
{
   /* make sure all data is processed */
   wait();

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


void OggEncoder::encodeCDAudio( const QByteArray &data )
{
   if( !encodeCDAudio( data.constData(), data.size() ) )
   {
      emit encodingFail();
   }
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


QWidget *OggEncoder::configWidget()
{
   return mpConfigWidget;
}


void OggEncoder::handleDotButton()
{
   Encoder::setDirectory( mpDirectory );
}
