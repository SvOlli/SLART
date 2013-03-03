/*
 * src/libs/Kryptonite/KryptoniteJobCoverDiscogs.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

/* class declaration */
#include "KryptoniteJobCoverDiscogs.hpp"

/* system headers */

/* Qt headers */
#include <QBuffer>

/* local library headers */

/* local headers */
#include "Kryptonite.hpp"

#include "../Common/Trace.hpp"


KryptoniteJobCoverDiscogs::KryptoniteJobCoverDiscogs( Kryptonite *kryptonite,
                                                        QObject *parent )
: KryptoniteJobCover( kryptonite, parent )
{
}


KryptoniteJobCoverDiscogs::~KryptoniteJobCoverDiscogs()
{
}


void KryptoniteJobCoverDiscogs::requestList( const QString &query )
{
   QUrl url( "http://www.discogs.com/search" );
   url.addQueryItem( "q", query );
   url.addQueryItem( "type", "all" );
   emit requestDownload( this, SLOT(parseListHtml(QByteArray)), url );
}


void KryptoniteJobCoverDiscogs::parseListHtml( const QByteArray &data )
{
   QRegExp imgRe( ".*<img[^>]*class=[^>]*search_result_image[^>]*src=\"([^\"]*)\".*" );
   QRegExp linkRe( ".*<a[^>]*class=[^>]*search_result_title[^>]*href=\"([^\"]*)\".*" );
   QString s( QString::fromUtf8(data.constData()) );
   QStringList l( s.split("\n", QString::SkipEmptyParts) );
   QUrl imgUrl;
   QUrl linkUrl;
   foreach( s, l )
   {
      if( s.contains( imgRe ) )
      {
         imgUrl.setUrl( QString(s).replace( imgRe, "\\1" ) );
      }
      if( s.contains( linkRe ) )
      {
         linkUrl.setUrl( QString(s).replace( linkRe, "http://www.discogs.com\\1" ) );
      }
      if( !imgUrl.isEmpty() && !linkUrl.isEmpty() )
      {
         emit requestDownload( this, SIGNAL(imageFound(QByteArray,QVariant)), imgUrl, QVariant( linkUrl ) );
         imgUrl.clear();
         linkUrl.clear();
      }
   }
}


void KryptoniteJobCoverDiscogs::requestImage( const QUrl &url, const QVariant &payload )
{
   emit requestDownload( this, SLOT(parseItemHtml(QByteArray,QVariant)), url, payload );
}


void KryptoniteJobCoverDiscogs::parseItemHtml( const QByteArray &data, const QVariant &payload )
{
   QRegExp imgRe( ".*<a href=\"([^\"]*)\">more images</a>.*" );
   QString s( QString::fromUtf8(data.constData()) );
   QStringList l( s.split("\n", QString::SkipEmptyParts) );
   foreach( s, l )
   {
      if( s.contains( imgRe ) )
      {
         QUrl imgUrl( QString(s).replace( imgRe, "http://www.discogs.com\\1" ) );
         emit requestDownload( this, SLOT(parseImagesHtml(QByteArray,QVariant)), imgUrl, payload );
      }
   }
}


void KryptoniteJobCoverDiscogs::parseImagesHtml(const QByteArray &data, const QVariant &payload)
{
   QRegExp imgRe( ".*<img[^>]*class=\"shadow image_frame\"[^>]*src=\"([^\"]*)\".*" );
   QString s( QString::fromUtf8(data.constData()) );
   QStringList l( s.split("\n", QString::SkipEmptyParts) );
   foreach( s, l )
   {
      if( s.contains( imgRe ) )
      {
         QUrl imgUrl( QString(s).replace( imgRe, "\\1" ) );
         emit requestDownload( this, SIGNAL(imageDownloaded(QByteArray,QVariant)), imgUrl, payload );
      }
   }
}
