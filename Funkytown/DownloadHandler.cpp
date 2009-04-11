/**
 * DownloadHandler.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#include "DownloadHandler.hpp"
#include "MySettings.hpp"
#include "ConfigDialog.hpp"

#include <QtGui>
#include <QtNetwork>

#include "MagicQueue.hpp"
#include "ScrollLine.hpp"
#include "TheMagic.hpp"

#include "Trace.hpp"


DownloadHandler::DownloadHandler( QWidget *parent )
: QWidget( parent )
, mDownloading( false )
, mHttpGetId( 0 )
, mpHttp( new QHttp( this ) )
, mpURL( new ScrollLine( this ) )
, mpFileName( new ScrollLine( this ) )
, mpProgressBar( new QProgressBar( this ) )
, mpTimer( new QTimer( this ) )
, mpMagicQueue( new MagicQueue( this ) )
, mpTheMagic( 0 )
{
   QBoxLayout *layout      = new QVBoxLayout( this );
   QGroupBox  *groupBox    = new QGroupBox( tr("Now Processing:"), this );
   QBoxLayout *groupLayout = new QVBoxLayout();
   
   if( MySettings().VALUE_SLARTCOMMUNICATION )
   {
      mpMagicQueue->setSelectionMode( QAbstractItemView::MultiSelection );
   }
   else
   {
      mpMagicQueue->setSelectionMode( QAbstractItemView::NoSelection );
   }
#if QT_VERSION < 0x040300
   layout->setMargin( 0 );
   groupLayout->setMargin( 5 );
#else
   layout->setContentsMargins( 0, 0, 0, 0 );
   groupLayout->setContentsMargins( 5, 5, 5, 5 );
#endif
   
   groupLayout->addWidget( mpURL );
   groupLayout->addWidget( mpFileName );
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
   if( (responseHeader.statusCode() >= 300) && (responseHeader.statusCode() < 400) )
   {
      mpTheMagic->mURL = responseHeader.value("location");
      mpMagicQueue->addMagic( mpTheMagic );
   }
   
   if( responseHeader.statusCode() != 200 )
   {
      errorMessage( mpTheMagic->mURL+QString(":")+QString::number( responseHeader.statusCode() )+
                    QString(" ")+responseHeader.reasonPhrase() );
      mAborting = true;
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
      return;
   }
   mpTimer->stop();
   
   mpTheMagic->preDownload();
   
   mpURL->setText( mpTheMagic->mURL );
   mpFileName->setText( mpTheMagic->fileName() );
   
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
      mHttpGetId = mpHttp->get( url.mid(slash1), mpTheMagic->ioDevice() );
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
   if ( requestId != mHttpGetId )
   {
      mpTimer->start();
      return;
   }

   if ( mAborting )
   {
      delete mpTheMagic;
      mpTheMagic = 0;
      mpTimer->start();
      return;
   }
   
   mpTheMagic->postDownload( !error );
   
   if( error )
   {
      mpMagicQueue->addMagic( mpTheMagic );
   }
   else
   {
      delete mpTheMagic;
   }
   mpTheMagic = 0;
   mpTimer->start();
}
