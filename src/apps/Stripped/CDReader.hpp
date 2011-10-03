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

/*!
  \addtogroup Stripped
  @{
  */


/*!
 \brief interface for reading data from cd

*/
class CDReader : public QWidget
{
   Q_OBJECT

public:
   /*!
    \brief constructor

    \param info
    \param edit
    \param parent
   */
   CDReader( CDInfo *info, CDEdit *edit, QWidget *parent = 0 );

   /*!
    \brief destructor

   */
   virtual ~CDReader();

   /*!
    \brief set the encoder to use

    \param encoders
   */
   void setEncoders( const MagicEncoderList &encoders );

   /*!
    \brief insert available devices into given combobox

   */
   void getDevices();

public slots:
   /*!
    \brief read the toc

   */
   void readToc();

   /*!
    \brief read the cd-text

   */
   void readCDText();

   /*!
    \brief rip tracks

   */
   void readTracks();

   /*!
    \brief set the device to use

    \param device
   */
   void setDevice( const QString &device );

   /*!
    \brief eject disc

   */
   void eject();

   /*!
    \brief handle cancel button

   */
   void cancel();

   /*!
    \brief (re-)read the settings

   */
   void readSettings();

   /*!
    \brief pop up that there's been an error and no eject

   */
   void noEject();

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
    \brief send a list of devices that have been found

    \param devices
   */
   void foundDevices( const QStringList &devices );

   /*!
    \brief send status message

    \param message
   */
   void message( const QString &message = QString() );

   /*!
    \brief send progress of track

    \param percent
   */
   void progress( int percent );

   /*!
    \brief track has been enabled / disabled

    \param track
    \param disabled
   */
   void setTrackDisabled( int track, bool disabled );

   /*!
    \brief make sure that the track is visible

    \param track
   */
   void ensureVisible( int track );

private:
   Q_DISABLE_COPY( CDReader )

   CDReaderThread       *mpCDReaderThread; /*!< TODO */
   CDInfo               *mpCDInfo; /*!< TODO */
   CDEdit               *mpCDEdit; /*!< TODO */
   QProgressBar         *mpProgressBar; /*!< TODO */
   ParanoiaStatus       *mpParanoiaStatus; /*!< TODO */
   MagicEncoderList     mEncoders; /*!< TODO */
   QString              mDevice; /*!< TODO */
};

/*! @} */

#endif
