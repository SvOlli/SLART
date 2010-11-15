/**
 * src/apps/Stripped/CDReaderThread.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef CDREADERTHREAD_HPP
#define CDREADERTHREAD_HPP CDREADERTHREAD_HPP

/* base class */
#include <QThread>

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

/* forward declaration of local classes */
class CDEdit;
class CDInfo;
class Encoder;


class CDReaderThread : public QThread
{
Q_OBJECT

public:
   CDReaderThread();
   virtual ~CDReaderThread();

   /* constructor replacement */
   void setup( CDInfo *info, CDEdit *edit,
               const MagicEncoderList &encoders, const QString &device );
   /* wrapper to get all cd-capable devices in an own thread */
   void startGetDevices();
   /* wrapper to read the toc in an own thread */
   void startReadToc();
   /* wrapper to read cd-text in an own thread */
   void startReadCDText();
   /* wrapper to read audio data in an own thread */
   void startReadAudioData();
   /* wrapper to eject the disc in an own thread */
   void startEject();
   /* implemented run of QThread */
   void run();

   /* callback for cdparanoia status */
   void callback( long inpos, ::paranoia_cb_mode_t function );

public slots:
   /* cancel the current action */
   void cancel();

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
   /* send current read errors of track */
   void errors( int track, unsigned int elements, const unsigned long *counts );
   /* send progress of track */
   void progress( int percent );
   /* track has been enabled/disabled */
   void setTrackDisabled( int track, bool disabled );
   /* make sure that the track is visible */
   void ensureVisible( int track );
   /* encode audio data to target formats */
   void encodeThis( const QByteArray &data );
   /* finalize encoding */
   void encodeDone();

private:
   CDReaderThread( const CDReaderThread &that );
   CDReaderThread &operator=( const CDReaderThread &that );

   /* real routine to get drives */
   void runGetDevices();
   /* real routine to read the toc */
   void runReadToc();
   /* real routine to read cd-text */
   void runReadCDText();
   /* real routine to read audio data */
   void runReadAudioData();
   /* real routine to eject CD from drive */
   void runEject();

   /* internal mode of operation, since there's only on run() */
   enum { modeUndefined, modeGetDevices, modeReadToc, modeReadCDText,
          modeReadAudioData, modeEject } mMode;

   ::CdIo_t             *mpCdIo;
   ::cdrom_drive_t      *mpDrive;
   ::cdrom_paranoia_t   *mpParanoia;
   CDInfo               *mpCDInfo;
   CDEdit               *mpCDEdit;
   unsigned long        *mpCallbackFunction;
   bool                 mCancel;
   bool                 mTrackHasErrors;
   QString              mDevice;
   QStringList          mDevices;
   MagicEncoderList     mEncoders;
};

#endif
