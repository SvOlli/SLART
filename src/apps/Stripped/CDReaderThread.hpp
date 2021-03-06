/*
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
#if CDIO_API_VERSION <= 5
#include <cdio/paranoia.h>
#else
#include <cdio/paranoia/paranoia.h>
#endif
}

/* Qt headers */
#include <QStringList>
#include <QMutex>

/* local library headers */
#include <MagicEncoderLoader.hpp>

/* local headers */

/* forward declaration of Qt classes */
class QTimer;

/* forward declaration of local classes */
class CDEdit;
class CDInfo;
class Encoder;

/*!
  \addtogroup Stripped
  @{
  */

/*!
 \brief worker thread for cd reading

 \dotfile "graphs/apps/Stripped/CDReaderThread_connect.dot" "Connect Graph"
*/
class CDReaderThread : public QThread
{
   Q_OBJECT

public:
   /*!
    \brief constructor

   */
   CDReaderThread();
   /*!
    \brief destructor

   */
   virtual ~CDReaderThread();

   /*!
    \brief constructor replacement as constructor does not take arguments

    \param info
    \param edit
    \param encoders
    \param device
   */
   void setup( CDInfo *info, CDEdit *edit,
               const MagicEncoderList &encoders, const QString &device );

   /*!
    \brief wrapper to get all cd-capable devices in an own thread

   */
   void startGetDevices();

   /*!
    \brief wrapper to read the toc in an own thread

   */
   void startReadToc();

   /*!
    \brief wrapper to read cd-text in an own thread

   */
   void startReadCDText();

   /*!
    \brief wrapper to read audio data in an own thread

   */
   void startReadAudioData();

   /*!
    \brief wrapper to eject the disc in an own thread

   */
   void startEject();

   /*!
    \brief implemented run of QThread

   */
   void run();

   /*!
    \brief callback for cdparanoia status

    \param inpos
    \param function
   */
   void callback( long inpos, ::paranoia_cb_mode_t function );

public slots:

   /*!
    \brief cancel the current action

   */
   void cancel();

signals:

   /*!
    \brief signalize "no disc"-state

   */
   void stateNoDisc();

   /*!
    \brief signalize "disc inserted"-state

   */
   void stateDisc();

   /*!
    \brief signalize "reading toc"-state

   */
   void stateScan();

   /*!
    \brief signalize "ripping audio"-state

   */
   void stateRip();

   /*!
    \brief signalize "toc read"-state

   */
   void gotToc();

   /*!
    \brief signalize "cdtext read"-state

   */
   void gotData();

   /*!
    \brief send a list of devices that've been found

    \param devices
   */
   void foundDevices( const QStringList &devices );

   /*!
    \brief send status message

    \param message
   */
   void message( const QString &message = QString() );

   /*!
    \brief send current read errors of track

    \param track
    \param elements
    \param counts
   */
   void errors( int track, unsigned int elements, const unsigned long *counts );

   /*!
    \brief signalize that disc has not been ejected because of errors

   */
   void noEject();

   /*!
    \brief send progress of track

    \param percent
    \param speed in sectors/s
   */
   void progress( int percent, int speed );

   /*!
    \brief track has been enabled/disabled

    \param track
    \param disabled
   */
   void setTrackDisabled( int track, bool disabled );

   /*!
    \brief make sure that the track is visible

    \param track
   */
   void ensureVisible( int track );

   /*!
    \brief encode audio data to target formats

    \param data
   */
   void encodeThis( const QByteArray &data );

   /*!
    \brief finalize encoding

   */
   void encodeDone();

private:
   Q_DISABLE_COPY( CDReaderThread )

   /*!
    \brief real routine to get drives

   */
   void runGetDevices();

   /*!
    \brief real routine to read the toc

   */
   void runReadToc();

   /*!
    \brief real routine to read cd-text

   */
   void runReadCDText();

   /*!
    \brief real routine to read audio data

   */
   void runReadAudioData();

   /*!
    \brief real routine to eject CD from drive

   */
   void runEject();

   /*!
    \brief internal mode of operation, since there's only on run()

   */
   enum { modeUndefined, modeGetDevices, modeReadToc, modeReadCDText,
          modeReadAudioData, modeEject } mMode; /*!< \brief \todo complete documentation */

   ::CdIo_t             *mpCdIo; /*!< \brief \todo complete documentation */
   ::cdrom_drive_t      *mpDrive; /*!< \brief \todo complete documentation */
   ::cdrom_paranoia_t   *mpParanoia; /*!< \brief \todo complete documentation */
   CDInfo               *mpCDInfo; /*!< \brief \todo complete documentation */
   CDEdit               *mpCDEdit; /*!< \brief \todo complete documentation */
   unsigned long        *mpCallbackFunction; /*!< \brief \todo complete documentation */
   bool                 mCancel; /*!< \brief \todo complete documentation */
   bool                 mTrackHasErrors; /*!< \brief \todo complete documentation */
   bool                 mDiscHasErrors; /*!< \brief \todo complete documentation */
   QString              mDevice; /*!< \brief \todo complete documentation */
   QStringList          mDevices; /*!< \brief \todo complete documentation */
   MagicEncoderList     mEncoders; /*!< \brief \todo complete documentation */
};

/*! @} */

#endif
