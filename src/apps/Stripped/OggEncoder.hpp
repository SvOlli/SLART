/**
 * src/apps/Stripped/OggEncoder.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef OGGENCODER_HPP
#define OGGENCODER_HPP OGGENCODER_HPP

/* base class */
#include "Encoder.hpp"

/* system headers */
extern "C" {
#include <vorbis/vorbisenc.h>
}

/* Qt headers */

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */
class QDoubleSpinBox;

/* forward declaration of local classes */





class OggEncoder : public Encoder
{
Q_OBJECT
   
public:
   OggEncoder( QWidget *parent = 0 );
   virtual ~OggEncoder();

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
   OggEncoder( const OggEncoder &other );
   OggEncoder &operator=( const OggEncoder &other );
   
   /* ogg initialize helper function call on first encode */
   bool oggInit();
   /* encode raw cd audio data */
   bool encodeCDAudio( const char *data, int size );
   
   QWidget              *mpConfigWidget;
   QDoubleSpinBox       *mpQuality;
   ::ogg_stream_state   mOggPagegStream;
   ::ogg_page           mOggPage;
   ::ogg_packet         mOggPacket;
   ::vorbis_info        mVorbisInfo;
   ::vorbis_comment     mVorbisComment;
   ::vorbis_dsp_state   mVorbisDspState;
   ::vorbis_block       mVorbisBlock;
   
   bool   mIsInit;
   float  mQuality;
};

#endif
