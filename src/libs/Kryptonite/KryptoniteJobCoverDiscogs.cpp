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
#include <QUrlQuery>

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
   emit message( tr( "Searching discogs.com" ) );
   QUrl url( "http://www.discogs.com/search" );
   QUrlQuery urlQuery;
   urlQuery.addQueryItem( "q", query );
   urlQuery.addQueryItem( "type", "all" );
   url.setQuery( urlQuery );
   emit requestDownload( this, SLOT(parseListHtml(QByteArray)), url );
}


void KryptoniteJobCoverDiscogs::parseListHtml( const QByteArray &data )
{
   emit message( tr( "Parsing discogs search reply" ), data );
   QRegExp imgRe( ".*<img[^>]*class=[^>]*search_result_image[^>]*src=\"([^\"]*)\".*" );
   QRegExp linkRe( ".*<a[^>]*class=[^>]*search_result_title[^>]*href=\"([^\"]*)\".*" );
   QString s( QString::fromUtf8(data.constData()) );
   QStringList l( s.split("\n", Qt::SkipEmptyParts) );
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
   emit message( tr( "Parsing discogs.com item page" ), data );
   //QRegExp imgRe( ".*<a href=\"([^\"]*)\">more images</a>.*" );
   QRegExp imgRe( ".*<a href=\"(/viewimages?[^\"]*)\">.*" );
   QString s( QString::fromUtf8(data.constData()) );
   QStringList l( s.split("\n", Qt::SkipEmptyParts) );
   foreach( s, l )
   {
      if( s.contains( imgRe ) )
      {
         QUrl imgUrl( QString(s).replace( imgRe, "http://www.discogs.com\\1" ) );
         emit requestDownload( this, SLOT(parseImagesHtml(QByteArray,QVariant)), imgUrl, payload );
         return;
      }
   }
}


void KryptoniteJobCoverDiscogs::parseImagesHtml( const QByteArray &data, const QVariant &payload )
{
   emit message( tr( "Parsing discogs.com images page" ), data );
   // <div class="image"><a href="/viewimages?release=753791"><img src="http://s.pixogs.com/image/R-150-753791-1155372972.jpeg" alt="Revolverheld - Revolverheld" />
   QRegExp imgRe1( ".*<img[^>]*class=\"shadow image_frame\"[^>]*src=\"([^\"]*)\".*" );
   QRegExp imgRe2( ".*<div class=\"image\">.*<img[^>]*src=\"([^\"]*)\".*" );
   QString s( QString::fromUtf8(data.constData()) );
   QStringList l( s.split("\n", Qt::SkipEmptyParts) );
   foreach( s, l )
   {
      if( s.contains( imgRe1 ) )
      {
         QUrl imgUrl( QString(s).replace( imgRe1, "\\1" ) );
         emit requestDownload( this, SIGNAL(imageDownloaded(QByteArray,QVariant)), imgUrl, payload );
         return;
      }
      else if( s.contains( imgRe2 ) )
      {
         QUrl imgUrl( QString(s).replace( imgRe2, "\\1" ) );
         emit requestDownload( this, SIGNAL(imageDownloaded(QByteArray,QVariant)), imgUrl, payload );
         return;
      }
   }
}
