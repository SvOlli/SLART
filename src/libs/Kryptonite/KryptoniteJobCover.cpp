/*
 * src/libs/Kryptonite/KryptoniteJobCover.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

/* class declaration */
#include "KryptoniteJobCover.hpp"

/* system headers */

/* Qt headers */

/* local library headers */

/* local headers */
#include "Kryptonite.hpp"


KryptoniteJobCover::KryptoniteJobCover( Kryptonite *kryptonite, QObject *parent )
: QObject( parent )
{
   connect( this, SIGNAL(requestDownload(QObject*,const char*,QUrl,QVariant)),
            kryptonite, SLOT(download(QObject*,const char*,QUrl,QVariant)) );
}


KryptoniteJobCover::~KryptoniteJobCover()
{
}


void KryptoniteJobCover::requestImage( KryptoniteJobCover *job,
                                       const QUrl &url, const QVariant &payload )
{
   if( !QMetaObject::invokeMethod( job, "requestImage", Qt::QueuedConnection,
                                   Q_ARG( QUrl, url ),
                                   Q_ARG( QVariant, payload ) ) )
   {
      qFatal( "%s:%d call failed in %s", __FILE__, __LINE__, Q_FUNC_INFO );
   }
}
