/**
 * src/apps/Funkytown/DownloadHandler.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "DownloadHandler.hpp"

/* system headers */

/* Qt headers */
#include <QtGui>
#include <QtNetwork>

/* local library headers */
#include <MySettings.hpp>
#include <ScrollLine.hpp>

/* local headers */
#include "ConfigDialog.hpp"
#include "MagicQueue.hpp"
#include "TheMagic.hpp"


#define USE_TRACE 0

#if USE_TRACE
#include <Trace.hpp>
#endif


DownloadHandler::DownloadHandler( QWidget *parent )
: QWidget( parent )
, mDownloading( false )
, mHttpGetId( 0 )
, mpHttp( new QHttp( this ) )
, mpURL( new ScrollLine( this ) )
, mpFileName( new ScrollLine( this ) )
, mpEnqueue( new QCheckBox( tr("Enqueue In Partyman"), this ) )
, mpProgressBar( new QProgressBar( this ) )
, mpTimer( new QTimer( this ) )
, mpMagicQueue( new MagicQueue( this ) )
, mpTheMagic( 0 )
, mCookieJar()
{
   QBoxLayout *layout      = new QVBoxLayout( this );
   QGroupBox  *groupBox    = new QGroupBox( tr("Now Processing:"), this );
   QBoxLayout *groupLayout = new QVBoxLayout();

   if( MySettings().VALUE_SLARTCOMMUNICATION )
   {
      mpMagicQueue->setSelectionMode( QAbstractItemView::MultiSelection );
      mpEnqueue->setEnabled( false );
   }
   else
   {
      mpMagicQueue->setSelectionMode( QAbstractItemView::NoSelection );
      mpEnqueue->setHidden( true );
   }
   layout->setContentsMargins( 0, 0, 0, 0 );
   layout->setSpacing( 1 );
   groupLayout->setContentsMargins( 5, 5, 5, 5 );
   groupLayout->setSpacing( 1 );

   groupLayout->addWidget( mpURL );
   groupLayout->addWidget( mpFileName );
   groupLayout->addWidget( mpEnqueue );
   groupLayout->addWidget( mpProgressBar );
   groupBox->setLayout( groupLayout );

   layout->addWidget( groupBox );
   layout->addWidget( mpMagicQueue );
   setLayout( layout );

   connect( mpHttp, SIGNAL(requestFinished(int, bool)),
            this, SLOT(httpRequestFinished(int, bool)) );
   connect( mpHttp, SIGNAL(dataReadProgress(int, int)),
            this, SLOT(updateDataReadProgress(int, int)) );
   connect( mpHttp, SIGNAL(responseHeaderReceived(const QHttpResponseHeader &)),
            this, SLOT(readResponseHeader(const QHttpResponseHeader &)) );
   connect( mpTimer, SIGNAL(timeout()),
            this, SLOT(startDownload()) );

   mpTimer->start( 1000 );
}


void DownloadHandler::handleEnqueue( bool checked )
{
   mpTheMagic->mSelected = checked;
}


void DownloadHandler::updateDataReadProgress( int bytesRead, int totalBytes )
{
   if ( mAborting )
   {
      return;
   }

   mpProgressBar->setMaximum(totalBytes);
   mpProgressBar->setValue(bytesRead);
}


void DownloadHandler::readResponseHeader( const QHttpResponseHeader &responseHeader )
{
   const QString contentType("Content-Type");
#if USE_TRACE
TRACESTART(DownloadHandler::readResponseHeader)
#endif
   if( (responseHeader.statusCode() >= 300) && (responseHeader.statusCode() < 400) )
   {
      TheMagic *magic = new TheMagic( *mpTheMagic );
      magic->mURL = responseHeader.value("location");
      mpMagicQueue->addMagic( magic );
      mpTheMagic->fail();
   }
   else if( responseHeader.statusCode() != 200 )
   {
      errorMessage( mpTheMagic->mURL+QString(":")+QString::number( responseHeader.statusCode() )+
                    QString(" ")+responseHeader.reasonPhrase() );
      mpTheMagic->fail();
   }

   if( responseHeader.hasContentType() )
   {
      mpTheMagic->setContentType( responseHeader.contentType() );
   }

   const QString setCookie("Set-Cookie");
   QList<QPair<QString, QString> > values( responseHeader.values() );
   for( int i = 0; i < values.size(); i++ )
   {
      if( !QString::compare(values.at(i).first, setCookie, Qt::CaseInsensitive) )
      {
#if USE_TRACE
TRACEMSG << values.at(i).second;
#endif
         mCookieJar.store( values.at(i).second );
      }
   }
}


void DownloadHandler::run( const QString &url )
{
   /* everything that's not http can't be downloaded */
   if( !url.startsWith("http://") )
   {
      return;
   }

   mpMagicQueue->addUrl( url );
}


void DownloadHandler::startDownload()
{
#if USE_TRACE
TRACESTART(DownloadHandler::startDownload)
#endif
   if( mpTheMagic )
   {
      return;
   }

   mpTheMagic = mpMagicQueue->getMagic();
   if( !mpTheMagic )
   {
      emit downloadActive( false );
      mpProgressBar->setValue(0);
      mpURL->clear();
      mpFileName->clear();
      mpEnqueue->setChecked( false );
      mpEnqueue->setEnabled( false );
      return;
   }
   mpTimer->stop();

   mpTheMagic->preDownload();

   mpURL->setText( mpTheMagic->mURL );
   mpFileName->setText( mpTheMagic->fileName() );
   mpEnqueue->setChecked( mpTheMagic->mSelected );
   mpEnqueue->setEnabled( true );

   if( mpTheMagic->ioDevice() )
   {
      emit downloadActive( true );

      mDownloading         = true;
      mAborting            = false;

      QString url( mpTheMagic->mURL );
      if( url.startsWith( "http://" ) )
      {
         url = url.mid( 7 );
      }
      int slash1 = url.indexOf('/');

      ProxyWidget::setProxy( mpHttp );
      mpHttp->setHost( url.left(slash1) );

      QString referer( mpTheMagic->referer() );
      QString userAgent( MySettings().VALUE_USERAGENT );
      QStringList cookies( mCookieJar.take( url.left(slash1), url.mid(slash1) ) );

      QHttpRequestHeader requestHeader( "GET", url.mid(slash1) );
      QList<QPair<QString, QString> > headers;
      headers.append( qMakePair(QString("Host"), url.left(slash1)) );
      if( referer.size() )
      {
         headers.append( qMakePair(QString("Referer"), referer) );
      }
      if( userAgent.size() )
      {
         headers.append( qMakePair( QString("User-Agent"), userAgent ) );
      }
      if( cookies.size() )
      {
         foreach( const QString &cookie, cookies )
         {
            headers.append( qMakePair( QString("Cookie"), cookie ) );
         }
      }
      requestHeader.setValues( headers );
#if USE_TRACE
TRACEMSG << requestHeader.toString();
#endif
      mHttpGetId = mpHttp->request( requestHeader, 0, mpTheMagic->ioDevice() );
   }
   else
   {
      mpTheMagic->postDownload( true );
      delete mpTheMagic;
      mpTheMagic = 0;
      mpTimer->start();
   }
}


void DownloadHandler::httpRequestFinished( int requestId, bool error )
{
#if USE_TRACE
TRACESTART(DownloadHandler::httpRequestFinished)
TRACEMSG << requestId << mHttpGetId;
#endif
   if( requestId != mHttpGetId )
   {
      return;
   }

   if( !mpTheMagic )
   {
      return;
   }

   mpTheMagic->postDownload( !error );

   if( error )
   {
      TheMagic *magic = new TheMagic( *mpTheMagic );
      mpMagicQueue->addMagic( magic );
   }
   delete mpTheMagic;
   mpTheMagic = 0;
   mpTimer->start();
}
