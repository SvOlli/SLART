/*
 * src/tests/TestAppWebServer/Downloader.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

/* class declaration */
#include "Downloader.hpp"

/* system headers */

/* Qt headers */
#include <QtGui>
#include <QMutexLocker>
#include <QNetworkAccessManager>
#include <QNetworkCookieJar>
#include <QNetworkReply>
#include <QNetworkRequest>

/* local library headers */
#include <Database.hpp>
#include <ProxyWidget.hpp>
#include <Sequence.hpp>
#include <Settings.hpp>

/* local headers */
#include "IODeviceStreamer.hpp"
#include "DownloadReply.hpp"

#include "Trace.hpp"


Downloader::Downloader( QObject *parent )
: QObject( parent )
, mpManager( new QNetworkAccessManager( this ) )
, mpCookieJar( new QNetworkCookieJar( this ) )
{
   ProxyWidget::setProxy( mpManager );
   mpManager->setCookieJar( mpCookieJar );
}


Downloader::~Downloader()
{
}


void Downloader::download( QObject *target, const char *slot,
                           const QUrl &url, const QVariant &payload )
{
   QNetworkRequest request;
   request.setUrl( url );
   new DownloadReply( mpManager->get( request ), target, slot, payload );
}


void Downloader::download( const QString &fileName, const QUrl &url )
{
   QFile *f = new QFile( fileName );
   if( f->open( QIODevice::WriteOnly ) )
   {
      download( f, url, true );
   }
   else
   {
      delete f;
   }
}


void Downloader::download( QIODevice *file, const QUrl &url, bool autoDelete )
{
   QNetworkRequest request;
   request.setUrl( url );
   new IODeviceStreamer( mpManager->get( request ), file, autoDelete );
}
