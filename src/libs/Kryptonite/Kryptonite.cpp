/*
 * src/libs/Kryptonite/Kryptonite.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

/* class declaration */
#include "Kryptonite.hpp"

/* system headers */

/* Qt headers */
#include <QtGui>
#include <QMutexLocker>
#include <QNetworkAccessManager>
#include <QNetworkCookieJar>
#include <QNetworkReply>
#include <QNetworkRequest>

/* local library headers */

/* local headers */
#include "IODeviceStreamer.hpp"
#include "KryptoniteReply.hpp"


Kryptonite::Kryptonite( QObject *parent )
: QObject( parent )
, mpManager( new QNetworkAccessManager( this ) )
, mpCookieJar( new QNetworkCookieJar( this ) )
{
   mpManager->setCookieJar( mpCookieJar );
}


Kryptonite::~Kryptonite()
{
}


QNetworkAccessManager *Kryptonite::networkAccessManager()
{
   return mpManager;
}


void Kryptonite::clear()
{
   QList<KryptoniteReply*> pendingReplies( findChildren<KryptoniteReply*>() );
   foreach( KryptoniteReply *k, pendingReplies )
   {
      k->deleteLater();
   }
}


QObject *Kryptonite::download( QObject *target, const char *slot,
                               const QUrl &url, const QVariant &payload )
{
   QNetworkRequest request( url );
   return new KryptoniteReply( mpManager, request, target, slot, payload );
}


QObject *Kryptonite::download( QObject *target, const char *slot,
                               const QNetworkRequest &request, const QVariant &payload )
{
   return new KryptoniteReply( mpManager, request, target, slot, payload );
}


QObject *Kryptonite::download( const QString &fileName, const QUrl &url )
{
   return download( fileName, QNetworkRequest( url ) );
}


QObject *Kryptonite::download( const QString &fileName, const QNetworkRequest &request )
{
   QFile *f = new QFile( fileName );
   if( f->open( QIODevice::WriteOnly ) )
   {
      return download( f, true, request );
   }
   else
   {
      delete f;
      return 0;
   }
}


QObject *Kryptonite::download( QIODevice *file, bool autoDelete, const QUrl &url )
{
   return download( file, autoDelete, QNetworkRequest( url ) );
}


QObject *Kryptonite::download( QIODevice *file, bool autoDelete, const QNetworkRequest &request )
{
   IODeviceStreamer *streamer = new IODeviceStreamer( mpManager->get( request ), file, autoDelete );
   connect( streamer, SIGNAL(destroyed(QObject*)),
            this, SIGNAL(downloadFinished(QObject*)) );
   emit downloadStarted( qobject_cast<QObject*>( file ), request );
   emit downloadStarted( qobject_cast<QObject*>( file ), request.url() );
   return streamer;
}
