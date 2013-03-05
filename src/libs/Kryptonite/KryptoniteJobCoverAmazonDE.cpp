/*
 * src/libs/Kryptonite/KryptoniteJobCoverAmazonDE.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

/* class declaration */
#include "KryptoniteJobCoverAmazonDE.hpp"

/* system headers */

/* Qt headers */
#include <QBuffer>

/* local library headers */

/* local headers */
#include "Kryptonite.hpp"

#include "../Common/Trace.hpp"


KryptoniteJobCoverAmazonDE::KryptoniteJobCoverAmazonDE( Kryptonite *kryptonite,
                                                        QObject *parent )
: KryptoniteJobCover( kryptonite, parent )
{
}


KryptoniteJobCoverAmazonDE::~KryptoniteJobCoverAmazonDE()
{
}


void KryptoniteJobCoverAmazonDE::requestList( const QString &query )
{
   emit message( tr( "Searching amazon.de" ) );
   QByteArray amazon( QByteArray::fromHex( "C3854DC3855AC395C391" ) );
   QUrl url( "http://www.amazon.de/s/ref=nb_sb_noss" );
   url.addQueryItem( "__mk_de_DE", QString::fromUtf8(amazon.constData()) );
   url.addQueryItem( "url", "search-alias=popular" );
   url.addQueryItem( "field-keywords", query );
   emit requestDownload( this, SLOT(parseListHtml(QByteArray)), url );
}


void KryptoniteJobCoverAmazonDE::parseListHtml( const QByteArray &data )
{
   emit message( tr( "Parsing amazon.de search reply" ), data );
   QRegExp imgRe( ".*<img[^>]*src=\"([^\"]*)\".*" );
   QRegExp linkRe( ".*<a[^>]*href=\"([^\"]*)\".*" );
   QString s( QString::fromUtf8(data.constData()) );
   QStringList l( s.split("\n", QString::SkipEmptyParts) );
   foreach( s, l )
   {
      if( s.contains("Produkt-Information") )
      {
         QUrl imgUrl( QString(s).replace( imgRe, "\\1" ) );
         QUrl linkUrl( QString(s).replace( linkRe, "\\1" ) );
         emit requestDownload( this, SIGNAL(imageFound(QByteArray,QVariant)), imgUrl, QVariant( linkUrl ) );
      }
   }
}


void KryptoniteJobCoverAmazonDE::requestImage( const QUrl &url, const QVariant &payload )
{
   emit requestDownload( this, SLOT(parseItemHtml(QByteArray,QVariant)), url, payload );
}


void KryptoniteJobCoverAmazonDE::parseItemHtml( const QByteArray &data, const QVariant &payload )
{
   emit message( tr( "Parsing amazon.de item page" ), data );
   QRegExp imgRe( ".*i=new Image;i.src = \"([^\"]*)\".*" );
   QString s( QString::fromUtf8(data.constData()) );
   QStringList l( s.split("\n", QString::SkipEmptyParts) );
   foreach( s, l )
   {
      if( s.contains( imgRe ) )
      {
         QUrl imgUrl( QString(s).replace( imgRe, "\\1" ) );
         emit requestDownload( this, SIGNAL(imageDownloaded(QByteArray,QVariant)), imgUrl, payload );
         return;
      }
   }
}
