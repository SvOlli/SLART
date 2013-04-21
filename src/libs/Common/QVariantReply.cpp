/*
 * QVariantReply.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "QVariantReply.hpp"

/* system headers */

/* Qt headers */

/* local library headers */

/* local headers */




QVariantReply::QVariantReply( QObject *target, const char *method )
: QObject( 0 )
{
   connect( this, SIGNAL(sendReply(QVariant)),
            target, method, Qt::QueuedConnection );
}


QVariantReply::~QVariantReply()
{
}


void QVariantReply::send( const QVariant &data )
{
   emit sendReply( data );
   deleteLater();
}
