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
#include <QUrlQuery>

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
   QUrlQuery urlQuery;
   urlQuery.addQueryItem( "__mk_de_DE", QString::fromUtf8(amazon.constData()) );
   urlQuery.addQueryItem( "url", "search-alias=popular" );
   urlQuery.addQueryItem( "field-keywords", query );
   url.setQuery( urlQuery );
   emit requestDownload( this, SLOT(parseListHtml(QByteArray)), url );
}


void KryptoniteJobCoverAmazonDE::parseListHtml( const QByteArray &data )
{
   emit message( tr( "Parsing amazon.de search reply" ), data );
   QRegExp imgRe( ".*<img[^>]*src=\"([^\"]*)\".*" );
   QRegExp linkRe( ".*<a[^>]*href=\"([^\"]*)\".*" );
   QString s( QString::fromUtf8(data.constData()) );
   QStringList l( s.split("\n", Qt::SkipEmptyParts) );
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
   // media%3Dhttp%253A%252F%252Fecx.images-amazon.com%252Fimages%252FI%252F41KzCF7bqML._SL500_.jpg%26
   emit message( tr( "Parsing amazon.de item page" ), data );
   //QRegExp imgRe1( ".*media%3D(https?)%253A%252F%252F([^%]*)%252F([^%]*)%252F([^%]*)%252F([^%]*.jpg)%26.*" );
   QRegExp imgRe1( ".*media%3D(.*.jpg)%26.*" );
   //imgRe1.setMinimal( true );
   QString s( QString::fromUtf8(data.constData()) );
   QStringList l( s.split("\n", Qt::SkipEmptyParts) );
   foreach( s, l )
   {
      if( s.contains( imgRe1 ) )
      {
         s.replace( imgRe1, "\\1" );
         while( s.contains('%') )
         {
            s = QUrl::fromPercentEncoding( s.toLatin1() );
         }
         QUrl imgUrl( s );
         //QUrl imgUrl( QString(s).replace( imgRe1, "\\1://\\2/\\3/\\4/\\5" ) );

         emit requestDownload( this, SIGNAL(imageDownloaded(QByteArray,QVariant)), imgUrl, payload );
         return;
      }
   }
   emit message( tr( "Using fallback search" ) );
   QRegExp imgRe2( ".*i=new Image;i.src = \"([^\"]*)\".*" );
   foreach( s, l )
   {
      if( s.contains( imgRe2 ) )
      {
         QUrl imgUrl( QString(s).replace( imgRe2, "\\1" ) );
         emit requestDownload( this, SIGNAL(imageDownloaded(QByteArray,QVariant)), imgUrl, payload );
         return;
      }
   }
}
