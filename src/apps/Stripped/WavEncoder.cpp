/**
 * src/apps/Stripped/WavEncoder.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#include "WavEncoder.hpp"
#include "TagList.hpp"

#include "Trace.hpp"

#include <QtGui>

WavEncoder::WavEncoder( QWidget *parent )
: Encoder( parent, tr("wav") )
, mpWavHeader( new unsigned int[11] )
{
   mpWavHeader[ 0] = 0x46464952; // "RIFF"
   mpWavHeader[ 1] = 0;          // wave size
   mpWavHeader[ 2] = 0x45564157; // "WAVE"
   mpWavHeader[ 3] = 0x20746D66; // "fmt "
   mpWavHeader[ 4] = 0x00000010; // size of "fmt "-header
   mpWavHeader[ 5] = 0x00020001; // format PCM / Stereo
   mpWavHeader[ 6] = 44100;      // samplerate
   mpWavHeader[ 7] = 44100 * 4;  // bytes per second
   mpWavHeader[ 8] = 0x00100004; // 16 bit / bytes per sample
   mpWavHeader[ 9] = 0x61746164; // "data"
   mpWavHeader[10] = 0;          // data size

   QHBoxLayout *mainLayout = new QHBoxLayout( this );
   QLabel      *label      = new QLabel( tr("no config"), this );

#if QT_VERSION < 0x040300
   mainLayout->setMargin( 0 );
#else
   mainLayout->setContentsMargins( 0, 0, 0, 0 );
#endif
   mainLayout->addWidget( label );

   setLayout( mainLayout );
}

WavEncoder::~WavEncoder()
{
   delete[] mpWavHeader;
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


bool WavEncoder::encodeCDAudio( const char* data, int size )
{
   return Encoder::writeChunk( data, size );
}
