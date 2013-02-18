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


class IODeviceStreamer : public QObject
{
   Q_OBJECT

public:
   IODeviceStreamer( QIODevice *reader, QIODevice *writer, bool autoDelete );
   virtual ~IODeviceStreamer();

   bool isSane();

public slots:
   void readChunck();

signals:

private:
   Q_DISABLE_COPY( IODeviceStreamer )

   QIODevice      *mpReader;
   QIODevice      *mpWriter;
};

#endif
