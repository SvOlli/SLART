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
#include <TagList.hpp>

/* local headers */


WavEncoder::WavEncoder( QWidget *parent )
: Encoder( parent, tr("wav") )
, mpConfigWidget( new QWidget( parent ) )
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

   QHBoxLayout *mainLayout = new QHBoxLayout( mpConfigWidget );
   QLabel      *label      = new QLabel( tr("no config"), mpConfigWidget );

#if QT_VERSION < 0x040300
   mainLayout->setMargin( 0 );
#else
   mainLayout->setContentsMargins( 0, 0, 0, 0 );
#endif
   mainLayout->addWidget( label );

   mpConfigWidget->setLayout( mainLayout );
}

WavEncoder::~WavEncoder()
{
   delete[] mpWavHeader;
}


void WavEncoder::readSettings()
{
}


void WavEncoder::writeSettings()
{
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
