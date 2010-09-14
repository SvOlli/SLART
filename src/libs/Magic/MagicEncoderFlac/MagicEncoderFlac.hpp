/**
 * src/apps/Magic/MagicEncoderFlac/MagicEncoderFlac.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef FLACENCODER_HPP
#define FLACENCODER_HPP FLACENCODER_HPP

/* base class */
#include "../MagicEncoderInterface.hpp"
#include <MagicEncoder.hpp>

/* system headers */
#include "FLAC++/metadata.h"
#include "FLAC++/encoder.h"

/* Qt headers */

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */
class QCheckBox;
class QPushButton;
class QSpinBox;

/* forward declaration of local classes */
class ScrollLine;


class MagicEncoderFlac : public MagicEncoder
{
Q_OBJECT
Q_INTERFACES(MagicEncoderInterface)

public:
   MagicEncoderFlac();
   virtual ~MagicEncoderFlac();

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
   MagicEncoderFlac( const MagicEncoderFlac &other );
   MagicEncoderFlac &operator=( const MagicEncoderFlac &other );

   /* encode raw cd audio data */
   bool encodeCDAudio( const char *data, int size );

   /* settings */
   int                  mQuality;
   bool                 mUseOga;
   /* configuration widget */
   QWidget              *mpConfigWidget;
   QCheckBox            *mpUseEncoder;
   QCheckBox            *mpDirOverride;
   ScrollLine           *mpDirectory;
   QPushButton          *mpDotButton;
   QSpinBox             *mpQuality;
   QCheckBox            *mpUseOga;
   /* encoder internal data */
   FLAC::Encoder::File  *mpEncoder;
   FLAC__StreamMetadata *mpMetadata;
   FLAC__int32          *mpPcm;
   int                  mSize;
};

#define VALUE_FLACQUALITY        value( "FlacQuality", 5 ).toInt()
#define VALUE_FLACUSEOGA         value( "FlacUseOga", false ).toBool()

#endif
