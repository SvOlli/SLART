/**
 * src/apps/Stripped/Mp3Encoder.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef MP3ENCODER_HPP
#define MP3ENCODER_HPP MP3ENCODER_HPP

/* base class */
#include "Encoder.hpp"

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


class Mp3Encoder : public Encoder
{
Q_OBJECT

public:
   Mp3Encoder( QWidget *parent = 0 );
   virtual ~Mp3Encoder();

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
   Mp3Encoder( const Mp3Encoder &other );
   Mp3Encoder &operator=( const Mp3Encoder &other );

   bool initialize();
   /* encode raw cd audio data */
   bool encodeCDAudio( const char *data, int size );

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

#endif
