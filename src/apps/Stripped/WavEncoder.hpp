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

   /*  */
   QWidget *configWidget();
   /* read settings from storage */
   void readSettings();
   /* write settings to storage */
   void writeSettings();
   /* initialize the encoder */
   bool initialize( const QString &fileName );
   /* finalize (clean up) the encoder */
   bool finalize( bool enqueue, bool cancel );

public slots:
   /* encode raw cd audio data */
   void encodeCDAudio( const QByteArray &data );
   
private:
   WavEncoder( const WavEncoder &other );
   WavEncoder &operator=( const WavEncoder &other );

   QWidget              *mpConfigWidget;
   unsigned int         *mpWavHeader;
};

#endif
