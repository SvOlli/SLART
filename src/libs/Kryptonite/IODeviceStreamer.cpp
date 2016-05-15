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
#include <QFile>
#include <QNetworkReply>

/* local library headers */

/* local headers */



IODeviceStreamer::IODeviceStreamer( QIODevice *reader, QIODevice *writer,
                                    bool autoDeleteWriter )
: QObject( reader )
, mpReader( reader )
, mpWriter( writer )
, mReaderSize( reader->size() )
, mPercent( 0 )
{
   if( reader->isSequential() )
   {
      // not possible
      mReaderSize = 0;
   }
   QNetworkReply *reply = qobject_cast<QNetworkReply*>( reader );
   if( reply )
   {
      connect( reply, SIGNAL(downloadProgress(qint64,qint64)),
               this, SLOT(updateProgress(qint64,qint64)) );
      mReaderSize = 0;
   }
   if( autoDeleteWriter )
   {
      connect( reader, SIGNAL(destroyed()),
               writer, SLOT(deleteLater()) );
   }
   connect( reader, SIGNAL(readyRead()),
            this, SLOT(readChunck()) );
}


IODeviceStreamer::~IODeviceStreamer()
{
   emit progress( 100 );
}


bool IODeviceStreamer::isSane()
{
   return (mpReader->openMode() == QIODevice::ReadOnly) && (mpWriter->openMode() & QIODevice::WriteOnly);
}


QString IODeviceStreamer::writeFileName() const
{
   QString retval;
   QFile *f = qobject_cast<QFile*>( mpWriter );
   if( f )
   {
      retval = f->fileName();
   }
   return retval;
}


void IODeviceStreamer::readChunck()
{
   if( mReaderSize )
   {
      updateProgress( mpReader->pos(), mReaderSize );
   }
   mpWriter->write( mpReader->readAll() );
   if( mpWriter->atEnd() )
   {
      deleteLater();
   }
}


void IODeviceStreamer::updateProgress( qint64 bytesReceived, qint64 bytesTotal )
{
   const int percent = 100 * bytesReceived / bytesTotal;
   if( mPercent != percent )
   {
      mPercent = percent;
      emit progress( mPercent );
   }
}
