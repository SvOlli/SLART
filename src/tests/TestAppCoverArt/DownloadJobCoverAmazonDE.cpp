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
   connect( this, SIGNAL(requestDownload(QObject*,const char*,QUrl)),
            downloader, SLOT(download(QObject*,const char*,QUrl)) );
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
   url.addQueryItem( "field-keywords", QString("%1 %2").arg( query ) );
   emit requestDownload( this, SLOT(parseHtml(QByteArray)), url );
}


void DownloadJobCoverAmazonDE::parseHtml( const QByteArray &data )
{
TRACESTART(DownloadJobCoverAmazonDE::parseHtml)

   QRegExp re( ".*<img[^>]*src=\"([^\"]*)\".*" );
   QString s( QString::fromUtf8(data.constData()) );
   QStringList l( s.split("\n", QString::SkipEmptyParts) );
   foreach( s, l )
   {
      if( s.contains("Produkt-Information") )
      {
         emit requestDownload( mpTarget, mpSlot, QUrl( s.replace( re, "\\1" ) ) );
      }
   }
}
