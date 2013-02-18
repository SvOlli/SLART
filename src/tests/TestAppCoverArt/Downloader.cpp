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

#include "Trace.hpp"

Downloader::Downloader( QObject *parent )
: QObject( parent )
, mpManager( new QNetworkAccessManager( this ) )
, mpCookieJar( new QNetworkCookieJar( this ) )
, mReplyMap()
, mObjects()
, mSlots()
, mDataMap()
{
   ProxyWidget::setProxy( mpManager );
   mpManager->setCookieJar( mpCookieJar );
   connect( mpManager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(handleFinished(QNetworkReply*)) );
}


Downloader::~Downloader()
{
}


#if 0
void Downloader::download( const QString &fileName, const QUrl &url )
{
   download( new QFile( fileName ), url, true );
}


void Downloader::download( QFile *file, const QUrl &url, bool autoDelete )
{
   QNetworkRequest request;
   request.setUrl( url );

   QNetworkReply *reply = mpManager->get( request );
   mDirectDownloadMap.insert( reply, file );
   mReplyMap.insert( reply, 0 );
   new IODeviceStreamer( reply, file, autoDelete );
}
#endif


void Downloader::download( QObject *target, const char *slot,
                           const QList<QUrl> &urlList )
{
   unsigned int id = Sequence::global();
   mObjects[id] = target;
   mSlots[id] = slot;

   foreach( const QUrl &url, urlList )
   {
      QNetworkRequest request;
      request.setUrl( url );
      //request.setRawHeader("User-Agent", "MyOwnBrowser 1.0");

      mReplyMap.insert( mpManager->get( request ), id );
   }
}


void Downloader::handleFinished( QNetworkReply *reply )
{
TRACESTART(Downloader::handleFinished)
   unsigned int id = mReplyMap.take( reply );
   // id == 0: direct download
   if( id )
   {
      mDataMap[id].append( reply->readAll() );
      reply->deleteLater();
      int number = mReplyMap.keys( id ).count();
      if( !number )
      {
         connect( this, SIGNAL(done(QList<QByteArray>)),
                  mObjects.take( id ), mSlots.take( id ) );
         emit done( mDataMap.take( id ) );
      }
   }
}
