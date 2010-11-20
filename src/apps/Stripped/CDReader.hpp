/**
 * src/apps/Stripped/CDReader.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef CDREADER_HPP
#define CDREADER_HPP CDREADER_HPP

/* base class */
#include <QWidget>

/* system headers */
extern "C" {
#include <cdio/cdio.h>
#include <cdio/paranoia.h>
}

/* Qt headers */
#include <QStringList>

/* local library headers */
#include <MagicEncoderLoader.hpp>

/* local headers */

/* forward declaration of Qt classes */
class QCheckBox;
class QComboBox;
class QLabel;
class QProgressBar;

/* forward declaration of local classes */
class CDEdit;
class CDInfo;
class CDReaderThread;
class Encoder;
class ParanoiaStatus;


class CDReader : public QWidget
{
Q_OBJECT

public:
   CDReader( CDInfo *info, CDEdit *edit, QWidget *parent = 0 );
   virtual ~CDReader();

   /* set the encoder to use */
   void setEncoders( const MagicEncoderList &encoders );
   /* insert available devices into given combobox */
   void getDevices();

public slots:
   /* read the toc */
   void readToc();
   /* read the cd-text */
   void readCDText();
   /* rip tracks */
   void readTracks();
   /* set the device to use */
   void setDevice( const QString &device );
   /* eject disc */
   void eject();
   /* handle cancel button */
   void cancel();
   /* (re-)read the settings */
   void readSettings();

signals:
   /* signalize "no disc"-state */
   void stateNoDisc();
   /* signalize "disc inserted"-state */
   void stateDisc();
   /* signalize "reading toc"-state */
   void stateScan();
   /* signalize "ripping audio"-state */
   void stateRip();
   /* signalize "toc read"-state */
   void gotToc();
   /* send a list of devices that've been found */
   void foundDevices( const QStringList &devices );
   /* send status message */
   void message( const QString &message = QString() );
   /* send progress of track */
   void progress( int percent );
   /* track has been enabled/disabled */
   void setTrackDisabled( int track, bool disabled );
   /* make sure that the track is visible */
   void ensureVisible( int track );

private:
   CDReader( const CDReader &that );
   CDReader &operator=( const CDReader &that );

   CDReaderThread       *mpCDReaderThread;
   CDInfo               *mpCDInfo;
   CDEdit               *mpCDEdit;
   QProgressBar         *mpProgressBar;
   ParanoiaStatus       *mpParanoiaStatus;
   MagicEncoderList     mEncoders;
   QString              mDevice;
};

#endif
