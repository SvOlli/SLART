/**
 * src/apps/Magic/MagicEncoderOgg/MagicEncoderOgg.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef OGGENCODER_HPP
#define OGGENCODER_HPP OGGENCODER_HPP

/* base class */
#include <MagicEncoder.hpp>

/* system headers */
extern "C" {
#include <vorbis/vorbisenc.h>
}

/* Qt headers */

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */
class QCheckBox;
class QDoubleSpinBox;
class QPushButton;

/* forward declaration of local classes */
class ScrollLine;


class MagicEncoderOgg : public MagicEncoder
{
Q_OBJECT
Q_INTERFACES(MagicEncoderInterface)

public:
   MagicEncoderOgg();
   virtual ~MagicEncoderOgg();

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
   /*  */
   virtual void setUseEncoder( bool on );
   /*  */
   void handleDotButton();
   
private:
   MagicEncoderOgg( const MagicEncoderOgg &other );
   MagicEncoderOgg &operator=( const MagicEncoderOgg &other );
   
   /* ogg initialize helper function call on first encode */
   bool oggInit();
   /* encode raw cd audio data */
   bool encodeCDAudio( const char *data, int size );
   
   /* settings */
   float                mQuality;
   /* configuration widget */
   QWidget              *mpConfigWidget;
   QCheckBox            *mpUseEncoder;
   QCheckBox            *mpDirOverride;
   ScrollLine           *mpDirectory;
   QPushButton          *mpDotButton;
   QDoubleSpinBox       *mpQuality;
   /* encoder internal data */
   bool                 mIsInit;
   ::ogg_stream_state   mOggPagegStream;
   ::ogg_page           mOggPage;
   ::ogg_packet         mOggPacket;
   ::vorbis_info        mVorbisInfo;
   ::vorbis_comment     mVorbisComment;
   ::vorbis_dsp_state   mVorbisDspState;
   ::vorbis_block       mVorbisBlock;
   
};

#define VALUE_OGGQUALITY         value( "OggQuality", 0.4 ).toDouble()

#endif
