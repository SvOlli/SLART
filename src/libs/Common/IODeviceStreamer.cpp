/*
 * src/tests/TestAppWebServer/IODeviceStreamer.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

/* class declaration */
#include "IODeviceStreamer.hpp"

/* system headers */

/* Qt headers */
#include <QIODevice>

/* local library headers */

/* local headers */



IODeviceStreamer::IODeviceStreamer( QIODevice *reader, QIODevice *writer, bool autoDelete )
: QObject( reader )
, mpReader( reader )
, mpWriter( writer )
{
   if( autoDelete )
   {
      connect( reader, SIGNAL(destroyed()),
               writer, SLOT(deleteLater()) );
   }
   connect( reader, SIGNAL(readyRead()),
            this, SLOT(readChunck()) );
}


IODeviceStreamer::~IODeviceStreamer()
{
}


bool IODeviceStreamer::isSane()
{
   return (mpReader->openMode() == QIODevice::ReadOnly) && (mpWriter->openMode() & QIODevice::WriteOnly);
}


void IODeviceStreamer::readChunck()
{
   mpWriter->write( mpReader->readAll() );
   if( mpWriter->atEnd() )
   {
      deleteLater();
   }
}
