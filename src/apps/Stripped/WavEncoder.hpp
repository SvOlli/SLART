/**
 * src/apps/Stripped/WavEncoder.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef WAVENCODER_HPP
#define WAVENCODER_HPP WAVENCODER_HPP

#include "Encoder.hpp"

#include <QWidget>


class WavEncoder : public Encoder
{
Q_OBJECT
   
public:
   WavEncoder( QWidget *parent = 0 );
   virtual ~WavEncoder();
   
   /* initialize the encoder */
   bool initialize( const QString &fileName );
   /* finalize (clean up) the encoder */
   bool finalize( bool enqueue, bool cancel );
   /* encode raw cd audio data */
   bool encodeCDAudio( const char* data, int size );
   
private:
   WavEncoder( const WavEncoder &other );
   WavEncoder &operator=( const WavEncoder &other );

   unsigned int   *mpWavHeader;
};

#endif
