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

extern "C"
{
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
}

#include <QtGui>

WavEncoder::WavEncoder( QWidget *parent )
: Encoder( parent, tr("wav") )
, mWavHeader( new unsigned int[11] )
{
   mWavHeader[ 0] = 0x46464952; // "RIFF"
   mWavHeader[ 1] = 0;          // wave size
   mWavHeader[ 2] = 0x45564157; // "WAVE"
   mWavHeader[ 3] = 0x20746D66; // "fmt "
   mWavHeader[ 4] = 0x00000010; // size of "fmt "-header
   mWavHeader[ 5] = 0x00020001; // format PCM / Stereo
   mWavHeader[ 6] = 44100;      // samplerate
   mWavHeader[ 7] = 44100 * 4;  // bytes per second
   mWavHeader[ 8] = 0x00100004; // 16 bit / bytes per sample
   mWavHeader[ 9] = 0x61746164; // "data"
   mWavHeader[10] = 0;          // data size

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
   delete[] mWavHeader;
}


bool WavEncoder::initialize( const QString &fileName )
{
   if( !Encoder::initialize( fileName, ".wav" ) )
   {
      return false;
   }
   mWavHeader[ 1] = 0;   // wave size
   mWavHeader[10] = 0;   // data size
   /* write now, what later will become the header */
   return mFile.write( (const char*)&mWavHeader[0], 44 );
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
   mWavHeader[ 1] = size - 8;   // wave size
   mWavHeader[10] = size - 44;  // data size
   /* data needs to be written as little endian */
   if( !mFile.write( (const char*)&mWavHeader[0], 44 ) )
   {
      Encoder::finalize( false, true );
      return false;
   }
   return Encoder::finalize( enqueue, cancel );
}


bool WavEncoder::setTags( const TagList &tagList )
{
   Q_UNUSED( tagList );
   /* wav does not support tags */
   return true;
}


bool WavEncoder::encodeCDAudio( const char* data, int size )
{
   return Encoder::writeChunk( data, size );
}
