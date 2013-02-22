/*
 * src/tests/TestAppWebServer/DownloadJobCoverAmazonDE.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

/* class declaration */
#include "DownloadJobCoverAmazonDE.hpp"

/* system headers */

/* Qt headers */
#include <QBuffer>

/* local library headers */
#include <Sequence.hpp>

/* local headers */
#include "Downloader.hpp"


#include "Trace.hpp"



DownloadJobCoverAmazonDE::DownloadJobCoverAmazonDE( Downloader *downloader,
                                                    QObject *target, const char *slot,
                                                    QObject *parent )
: QObject( parent )
, mpTarget( target )
, mpSlot( slot )
{
   connect( this, SIGNAL(requestDownload(QObject*,const char*,QUrl,QVariant)),
            downloader, SLOT(download(QObject*,const char*,QUrl,QVariant)) );
}


DownloadJobCoverAmazonDE::~DownloadJobCoverAmazonDE()
{
}


void DownloadJobCoverAmazonDE::query( const QString &query )
{
TRACESTART(DownloadJobCoverAmazonDE::query)

   QByteArray amazon( QByteArray::fromHex( "C3854DC3855AC395C391" ) );
   QUrl url( "http://www.amazon.de/s/ref=nb_sb_noss" );
   url.addQueryItem( "__mk_de_DE", QString::fromUtf8(amazon.constData()) );
   url.addQueryItem( "url", "search-alias=popular" );
   url.addQueryItem( "field-keywords", query );
   emit requestDownload( this, SLOT(parseHtml(QByteArray,QVariant)), url, QVariant( url ) );
}


void DownloadJobCoverAmazonDE::parseHtml( const QByteArray &data, const QVariant &payload )
{
TRACESTART(DownloadJobCoverAmazonDE::parseHtml)

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
         emit requestDownload( mpTarget, mpSlot, imgUrl, QVariant( linkUrl ) );
      }
   }
}
// <img src="http://ecx.images-amazon.com/images/I/51M22V8MHYL._SL500_AA300_.jpg" id="original-main-image"
