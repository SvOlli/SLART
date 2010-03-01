/**
 * src/apps/Stripped/WavEncoder.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef WAVENCODER_HPP
#define WAVENCODER_HPP WAVENCODER_HPP

/* base class */
#include "Encoder.hpp"

/* system headers */

/* Qt headers */

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */

/* forward declaration of local classes */



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
   /* read settings from storage */
   void readSettings();
   /* write settings to storage */
   void writeSettings();
   
private:
   WavEncoder( const WavEncoder &other );
   WavEncoder &operator=( const WavEncoder &other );

   unsigned int   *mpWavHeader;
};

#endif
