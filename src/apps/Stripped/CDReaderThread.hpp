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

   /*  */
   void setup( CDInfo *info, CDEdit *edit,
               const MagicEncoderList &encoders, const QString &device );
   /*  */
   void startGetDevices();
   /*  */
   void startReadToc();
   /*  */
   void startReadCDText();
   /*  */
   void startReadAudioData();
   /*  */
   void startEject();
   /*  */
   void run();

   /*  */
   void callback( long inpos, ::paranoia_cb_mode_t function );

public slots:
   /*  */
   void cancel();

signals:
   /*  */
   void stateNoDisc();
   /*  */
   void stateDisc();
   /*  */
   void stateScan();
   /*  */
   void stateRip();
   /*  */
   void gotToc();
   /*  */
   void foundDevices( const QStringList &devices );
   /*  */
   void message( const QString &message = QString() );
   /*  */
   void errors( int track, unsigned int elements, const unsigned long *counts );
   /*  */
   void progress( int percent );
   /*  */
   void setTrackDisabled( int track, bool disabled );
   /*  */
   void ensureVisible( int track );
   /*  */
   void encodeThis( const QByteArray &data );
   /*  */
   void encodeDone();


private:
   CDReaderThread( const CDReaderThread &other );
   CDReaderThread &operator=( const CDReaderThread &other );

   /*  */
   void runGetDevices();
   /*  */
   void runReadToc();
   /*  */
   void runReadCDText();
   /*  */
   void runReadAudioData();
   /*  */
   void runEject();

   /*  */
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
