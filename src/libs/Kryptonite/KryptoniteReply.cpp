/*
 * src/libs/Kryptonite/KryptoniteReply.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

/* class declaration */
#include "KryptoniteReply.hpp"

/* system headers */

/* Qt headers */
#include <QNetworkReply>
#include <QTimer>

/* local library headers */

/* local headers */



KryptoniteReply::KryptoniteReply( QNetworkAccessManager *manager,
                                  const QNetworkRequest &request,
                                  QObject *target, const char *slot,
                                  const QVariant &payload )
: QObject( manager )
, mpManager( manager )
, mpReply( 0 )
, mRequest( request )
, mPayload( payload )
{
   setObjectName( request.url().url() );
   connect( this, SIGNAL(complete(QByteArray,QVariant)),
            target, slot );
   connect( target, SIGNAL(destroyed()),
            this, SLOT(deleteLater()) );
   QTimer::singleShot( 1, this, SLOT(request()) );
}


void KryptoniteReply::request()
{
   mpReply = mpManager->get( mRequest );
   connect( mpReply, SIGNAL(error(QNetworkReply::NetworkError)),
            mpReply, SLOT(deleteLater()) );
   connect( mpReply, SIGNAL(finished()),
            this, SLOT(getData()) );
   connect( mpReply, SIGNAL(destroyed()),
            this, SLOT(deleteLater()) );
}


KryptoniteReply::~KryptoniteReply()
{
}


void KryptoniteReply::getData()
{
   if( mpReply->bytesAvailable() > 0 )
   {
      emit complete( mpReply->readAll(), mPayload );
   }
   mpReply->deleteLater();
}
