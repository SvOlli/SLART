/*
 * src/tests/TestAppWebServer/IODeviceStreamer.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef IODEVICESTREAMER_HPP
#define IODEVICESTREAMER_HPP IODEVICESTREAMER_HPP

/* base class */
#include <QObject>

/* system headers */

/* Qt headers */

/* local library headers */
#include <TrackInfo.hpp>

/* local headers */

/* forward declaration of Qt classes */
class QIODevice;

/* forward declaration of local classes */


/*!
 \brief copy data from one file to another

 primary use is the file download function of \ref Downloader

 \dotfile "graphs/libs/Common/IODeviceStreamer_connect.dot" "Connect Graph"
*/
class IODeviceStreamer : public QObject
{
   Q_OBJECT

public:
   /*!
    \brief constructor

    \param reader
    \param writer
    \param autoDelete should writer be deleted after completion
   */
   IODeviceStreamer( QIODevice *reader, QIODevice *writer, bool autoDelete );
   /*!
    \brief destructor

    will be called internally after completion
   */
   virtual ~IODeviceStreamer();

   /*!
    \brief a simple check if reader and writer are correctly configured

    \return bool
   */
   bool isSane();

public slots:
   /*!
    \brief slot for copying available data from reader to writer

   */
   void readChunck();

signals:

private:
   Q_DISABLE_COPY( IODeviceStreamer )

   QIODevice      *mpReader; /*!< \brief reader */
   QIODevice      *mpWriter; /*!< \brief writer */
};

#endif
