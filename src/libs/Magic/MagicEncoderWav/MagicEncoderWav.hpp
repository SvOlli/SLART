/**
 * src/apps/Magic/MagicEncoderWav/MagicEncoderWav.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef WAVENCODER_HPP
#define WAVENCODER_HPP WAVENCODER_HPP

/* base class */
#include <MagicEncoder.hpp>

/* system headers */

/* Qt headers */

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */
class QCheckBox;
class QPushButton;

/* forward declaration of local classes */
class ScrollLine;


class MagicEncoderWav : public MagicEncoder
{
Q_OBJECT
Q_INTERFACES(MagicEncoderInterface)

public:
   MagicEncoderWav();
   virtual ~MagicEncoderWav();

   /* supply the a handle to the configuration widget */
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
   /* set if the encoder should be used */
   virtual void setUseEncoder( bool on );
   /* handle the dot button by display filesystem browser */
   void handleDotButton();

private:
   MagicEncoderWav( const MagicEncoderWav &other );
   MagicEncoderWav &operator=( const MagicEncoderWav &other );

   /* configuration widget */
   QWidget              *mpConfigWidget;
   QCheckBox            *mpUseEncoder;
   QCheckBox            *mpDirOverride;
   ScrollLine           *mpDirectory;
   QPushButton          *mpDotButton;

   unsigned int         *mpWavHeader;
};

#endif
