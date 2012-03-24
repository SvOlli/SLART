/*
 * src/apps/Magic/MagicEncoderWav/MagicEncoderWav.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

/* class declaration */
#include "MagicEncoderWav.hpp"

/* system headers */

/* Qt headers */
#include <QtGui>

/* local library headers */
#include <ScrollLine.hpp>
#include <Settings.hpp>
#include <TagList.hpp>

/* local headers */


MagicEncoderWav::MagicEncoderWav()
: MagicEncoder( tr("wav") )
, mpConfigWidget()
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
}


MagicEncoderWav::~MagicEncoderWav()
{
   if( mpConfigWidget )
   {
      delete mpConfigWidget;
   }
   delete[] mpWavHeader;
}


bool MagicEncoderWav::initialize( const QString &fileName )
{
   if( !MagicEncoder::initialize( fileName, ".wav" ) )
   {
      return false;
   }
   mpWavHeader[ 1] = 0;   // wave size
   mpWavHeader[10] = 0;   // data size
   /* write now, what later will become the header */
   return mFile.write( (const char*)&mpWavHeader[0], 44 );
}


bool MagicEncoderWav::finalize( bool enqueue, bool cancel )
{
   /* make sure all data is processed */
   wait();

   unsigned int size = (mFile.pos() & 0xFFFFFFFF);
   /* write the wave header */
   if( !mFile.seek( 0 ) )
   {
      MagicEncoder::finalize( false, true );
      return false;
   }
   mpWavHeader[ 1] = size - 8;   // wave size
   mpWavHeader[10] = size - 44;  // data size
   /* data needs to be written as little endian */
   if( !mFile.write( (const char*)&mpWavHeader[0], 44 ) )
   {
      MagicEncoder::finalize( false, true );
      return false;
   }
   return MagicEncoder::finalize( enqueue, cancel );
}


void MagicEncoderWav::encodeCDAudio( const QByteArray &data )
{
   if (!MagicEncoder::writeChunk( data.constData(), data.size() ) )
   {
      emit encodingFail();
   }
}


MagicEncoderConfig *MagicEncoderWav::configWidget( QWidget *parent )
{
   if( !mpConfigWidget )
   {
      mpToggleEnableAction = new QAction( mName, parent );
      mpConfigWidget = new MagicEncoderWavConfig( this, parent, mpToggleEnableAction );
   }
   return mpConfigWidget;
}


Q_EXPORT_PLUGIN2(MagicEncoder, MagicEncoderWav)
