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

/* local library headers */
#include <Sequence.hpp>

/* local headers */
#include "Downloader.hpp"


#include "Trace.hpp"


DownloadJobCoverAmazonDE::DownloadJobCoverAmazonDE( Downloader *downloader,
                                                    QObject *parent )
: QObject( parent )
, mpObject( 0 )
, mpSlot( 0 )
{
   connect( this, SIGNAL(requestDownload(QObject*,const char*,QList<QUrl>)),
            downloader, SLOT(download(QObject*,const char*,QList<QUrl>)) );
}


DownloadJobCoverAmazonDE::~DownloadJobCoverAmazonDE()
{
}


void DownloadJobCoverAmazonDE::setTarget( QObject *object, const char *slot )
{
   mpObject = object;
   mpSlot = slot;
}


void DownloadJobCoverAmazonDE::query( const QString &query )
{
   if( !mpObject || !mpSlot )
   {
      // consumer not set
      return;
   }
   QList<QUrl> urlList;
   QByteArray amazon( QByteArray::fromHex( "C3854DC3855AC395C391" ) );
   QUrl url( "http://www.amazon.de/s/ref=nb_sb_noss" );
   url.addQueryItem( "__mk_de_DE", QString::fromUtf8(amazon.constData()) );
   url.addQueryItem( "url", "search-alias=popular" );
   url.addQueryItem( "field-keywords", QString("%1 %2").arg( query ) );
   urlList.append( url );
   emit requestDownload( this, SLOT(parseHtml(QList<QByteArray>)), urlList );
}


void DownloadJobCoverAmazonDE::parseHtml( const QList<QByteArray> &data )
{
   QList<QUrl> urlList;
   QRegExp re( ".*<img[^>]*src=\"([^\"]*)\".*" );
   foreach( const QByteArray &d, data )
   {
      QString s( QString::fromUtf8(d.constData()) );
      QStringList l( s.split("\n", QString::SkipEmptyParts) );
      foreach( s, l )
      {
         if( s.contains("Produkt-Information") )
         {
            urlList << QUrl( s.replace( re, "\\1" ) );
         }
      }
   }
   emit requestDownload( mpObject, mpSlot, urlList );
}
