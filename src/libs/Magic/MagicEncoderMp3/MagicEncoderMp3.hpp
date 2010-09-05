/**
 * src/apps/Magic/MagicEncoderMp3/MagicEncoderMp3.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef MP3ENCODER_HPP
#define MP3ENCODER_HPP MP3ENCODER_HPP

/* base class */
#include <MagicEncoder.hpp>

/* system headers */
extern "C" {
#include <lame/lame.h>
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


class MagicEncoderMp3 : public MagicEncoder
{
Q_OBJECT
Q_INTERFACES(MagicEncoder)

public:
   MagicEncoderMp3( QWidget *parent = 0 );
   virtual ~MagicEncoderMp3();

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
   MagicEncoderMp3( const MagicEncoderMp3 &other );
   MagicEncoderMp3 &operator=( const MagicEncoderMp3 &other );

   bool initialize();
   /* encode raw cd audio data */
   bool encodeCDAudio( const char *data, int size );
   /* convert tag #i to latin1 or utf8 */
   QByteArray tagTo8Bit( int i );

   /* settings */
   bool                 mUseAbr;
   bool                 mUseLatin1;
   float                mQuality;
   /* configuration widget */
   QWidget              *mpConfigWidget;
   QCheckBox            *mpUseEncoder;
   QCheckBox            *mpDirOverride;
   ScrollLine           *mpDirectory;
   QPushButton          *mpDotButton;
   QDoubleSpinBox       *mpQuality;
   QCheckBox            *mpUseLatin1;
   /* encoder internal data */
   volatile lame_t      mLame;
   const int            mMp3BufferSize;
   unsigned char        *mpMp3Buffer;
};

#define VALUE_USE_LATIN1         value( "UseLatin1", false ).toBool()
#define VALUE_VBRQUALITY         value( "VBRQuality", 4.0 ).toDouble()

#endif
