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
{
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
}


void WavEncoder::initialize( const QString &fileName )
{
TRACESTART(WavEncoder::initialize)
   char dummy[44];
   Encoder::initialize( fileName, ".wav" );
   /* write now, what later will become the header */
   ::memset( &dummy[0], 0, 44 );
   ::write( mFD, &dummy[0], 44 );
}


void WavEncoder::finalize( bool enqueue, bool cancel )
{
TRACESTART(WavEncoder::finalize)
   unsigned int size = (unsigned int) ::lseek( mFD, 0, SEEK_CUR );
   /* write the wave header */
   ::lseek( mFD, 0, SEEK_SET );
   unsigned int header[11];
   header[ 0] = 0x46464952; // "RIFF"
   header[ 1] = size - 8;   // wave size
   header[ 2] = 0x45564157; // "WAVE"
   header[ 3] = 0x20746D66; // "fmt "
   header[ 4] = 0x00000010; // size of "fmt "-header
   header[ 5] = 0x00020001; // format PCM / Stereo
   header[ 6] = 44100;      // samplerate
   header[ 7] = 44100 * 4;  // bytes per second
   header[ 8] = 0x00100004; // 16 bit / bytes per sample
   header[ 9] = 0x61746164; // "data"
   header[10] = size - 44;  // data size
   /* data needs to be written as little endian */
   ::write( mFD, &header[0], 44 );
   Encoder::finalize( enqueue, cancel );
}


void WavEncoder::setTags( const TagList &/*tagList*/ )
{
}


bool WavEncoder::encodeCDAudio( const char* data, int size )
{
   while( size > 0 )
   {
      int written = ::write( mFD, data, size );
      if( written < 0 )
      {
         return false;
      }
      size -= written;
      data += written;
   }
   return true;
}
