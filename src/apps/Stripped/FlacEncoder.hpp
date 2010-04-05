/**
 * src/apps/Stripped/FlacEncoder.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef FLACENCODER_HPP
#define FLACENCODER_HPP FLACENCODER_HPP

/* base class */
#include "Encoder.hpp"

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


class FlacEncoder : public Encoder
{
Q_OBJECT

public:
   FlacEncoder( QWidget *parent = 0 );
   virtual ~FlacEncoder();

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
   FlacEncoder( const FlacEncoder &other );
   FlacEncoder &operator=( const FlacEncoder &other );

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

#endif
