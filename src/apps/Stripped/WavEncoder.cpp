/**
 * src/apps/Stripped/WavEncoder.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "WavEncoder.hpp"

/* system headers */

/* Qt headers */
#include <QtGui>

/* local library headers */
#include <MySettings.hpp>
#include <ScrollLine.hpp>
#include <TagList.hpp>

/* local headers */
#include "ConfigDialog.hpp"


WavEncoder::WavEncoder( QWidget *parent )
: Encoder( tr("wav") )
, mpConfigWidget( new QWidget( parent ) )
, mpUseEncoder( new QCheckBox( tr("Use This Encoder"), mpConfigWidget ) )
, mpDirOverride( new QCheckBox( tr("Override Base Directory"), mpConfigWidget ) )
, mpDirectory( new ScrollLine( mpConfigWidget ) )
, mpDotButton( new QPushButton( tr("..."), mpConfigWidget ) )
, mpWavHeader( new unsigned int[11] )
{
   mpWavHeader[ 0] = qToBigEndian( 0x52494646 ); // "RIFF"
   mpWavHeader[ 1] = 0;                          // wave size
   mpWavHeader[ 2] = qToBigEndian( 0x57415645 ); // "WAVE"
   mpWavHeader[ 3] = qToBigEndian( 0x666D7420 ); // "fmt "
   mpWavHeader[ 4] = 0x00000010;                 // size of "fmt "-header
   mpWavHeader[ 5] = 0x00020001;                 // format PCM / Stereo
   mpWavHeader[ 6] = 44100;                      // samplerate
   mpWavHeader[ 7] = 44100 * 4;                  // bytes per second
   mpWavHeader[ 8] = 0x00100004;                 // 16 bit / bytes per sample
   mpWavHeader[ 9] = qToBigEndian( 0x64617461 ); // "data"
   mpWavHeader[10] = 0;                          // data size

   QGridLayout *mainLayout = new QGridLayout( mpConfigWidget );
   mainLayout->setContentsMargins( 3, 3, 3, 3 );
   mainLayout->addWidget( mpUseEncoder, 0, 0, 1, 3 );
   mainLayout->addWidget( mpDirOverride, 1, 0, 1, 3 );
   mainLayout->addWidget( new QLabel( tr("Base Directory:") ), 2, 0 );
   mainLayout->addWidget( mpDirectory, 2, 1 );
   mainLayout->addWidget( mpDotButton, 2, 2 );
   mainLayout->setColumnStretch( 1, 1 );
   mainLayout->setRowStretch( 3, 1 );

   connect( mpUseEncoder, SIGNAL(clicked(bool)),
            this, SIGNAL(useEncoderClicked(bool)) );
   connect( mpDotButton, SIGNAL(clicked()),
            this, SLOT(handleDotButton()) );

   mpConfigWidget->setLayout( mainLayout );
}


WavEncoder::~WavEncoder()
{
   delete[] mpWavHeader;
}


void WavEncoder::setUseEncoder( bool on )
{
   mpUseEncoder->setChecked( on );
}


void WavEncoder::readSettings()
{
   MySettings settings;
   settings.beginGroup( mName );
   mUseEncoder  = settings.VALUE_USE_ENCODER;
   mDirOverride = settings.VALUE_DIRECTORY_OVERRIDE;
   mDirectory   = settings.VALUE_DIRECTORY;
   mpUseEncoder->setChecked( mUseEncoder );
   mpDirOverride->setChecked( mDirOverride );
   mpDirectory->setText( mDirectory );
   settings.endGroup();
}


void WavEncoder::writeSettings()
{
   MySettings settings;
   settings.beginGroup( mName );
   mUseEncoder  = mpUseEncoder->isChecked();
   mDirOverride = mpDirOverride->isChecked();
   mDirectory   = mpDirectory->text();
   settings.setValue( "UseEncoder", mUseEncoder );
   settings.setValue( "DirectoryOverride", mDirOverride );
   settings.setValue( "Directory", mDirectory );
   settings.endGroup();
}


bool WavEncoder::initialize( const QString &fileName )
{
   if( !Encoder::initialize( fileName, ".wav" ) )
   {
      return false;
   }
   mpWavHeader[ 1] = 0;   // wave size
   mpWavHeader[10] = 0;   // data size
   /* write now, what later will become the header */
   return mFile.write( (const char*)&mpWavHeader[0], 44 );
}


bool WavEncoder::finalize( bool enqueue, bool cancel )
{
   /* make sure all data is processed */
   wait();

   unsigned int size = (mFile.pos() & 0xFFFFFFFF);
   /* write the wave header */
   if( !mFile.seek( 0 ) )
   {
      Encoder::finalize( false, true );
      return false;
   }
   mpWavHeader[ 1] = size - 8;   // wave size
   mpWavHeader[10] = size - 44;  // data size
   /* data needs to be written as little endian */
   if( !mFile.write( (const char*)&mpWavHeader[0], 44 ) )
   {
      Encoder::finalize( false, true );
      return false;
   }
   return Encoder::finalize( enqueue, cancel );
}


void WavEncoder::encodeCDAudio( const QByteArray &data )
{
   if (!Encoder::writeChunk( data.constData(), data.size() ) )
   {
      emit encodingFail();
   }
}


QWidget *WavEncoder::configWidget()
{
   return mpConfigWidget;
}


void WavEncoder::handleDotButton()
{
   Encoder::setDirectory( mpDirectory );
}
