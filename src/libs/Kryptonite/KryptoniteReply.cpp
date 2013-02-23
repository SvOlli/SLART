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

/* local library headers */

/* local headers */



KryptoniteReply::KryptoniteReply( QNetworkReply *reply,
                                  QObject *target, const char *slot,
                                  const QVariant &payload )
: QObject( reply )
, mpReply( reply )
, mPayload( payload )
{
   connect( reply, SIGNAL(finished()),
            this, SLOT(getData()) );
   connect( this, SIGNAL(complete(QByteArray,QVariant)),
            target, slot );
   connect( reply, SIGNAL(error(QNetworkReply::NetworkError)),
            reply, SLOT(deleteLater()) );
   connect( target, SIGNAL(destroyed()),
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
