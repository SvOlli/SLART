/**
 * DownloadHandler.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#include "DownloadHandler.hpp"
#include "PostDownloadHandler.hpp"
#include "MySettings.hpp"

#include <QtGui>
#include <QtNetwork>

#include "Trace.hpp"


DownloadHandler::DownloadHandler( QWidget *parent )
: QWidget( parent )
, mDownloading( false )
, mHttpGetId( 0 )
, mpHttp( new QHttp( this ) )
, mpFile( 0 )
, mpProgressBar( new QProgressBar( this ) )
, mpQueue( new QListWidget( this ) )
, mpTimer( new QTimer( this ) )
{
   QGridLayout *layout = new QGridLayout( this );
   if( MySettings().value( "SLARTCommunication", false ).toBool() )
   {
      mpQueue->setSelectionMode( QAbstractItemView::MultiSelection );
   }
   else
   {
      mpQueue->setSelectionMode( QAbstractItemView::NoSelection );
   }
#if QT_VERSION < 0x040300
   layout->setMargin( 0 );
#else
   layout->setContentsMargins( 0, 0, 0, 0 );
#endif
   layout->addWidget( mpProgressBar );
   layout->addWidget( mpQueue );
   setLayout( layout );
   
   connect( mpHttp, SIGNAL(requestFinished(int, bool)),
            this, SLOT(httpRequestFinished(int, bool)) );
   connect( mpHttp, SIGNAL(dataReadProgress(int, int)),
            this, SLOT(updateDataReadProgress(int, int)) );
   connect( mpHttp, SIGNAL(responseHeaderReceived(const QHttpResponseHeader &)),
            this, SLOT(readResponseHeader(const QHttpResponseHeader &)) );
   connect( mpTimer, SIGNAL(timeout()),
            this, SLOT(startDownload()) );
   
   mpTimer->start(1000);
}


void DownloadHandler::httpRequestFinished( int requestId, bool error )
{
   QListWidgetItem *qlwi;
   bool selected = false;
   
   if ( requestId != mHttpGetId )
   {
      return;
   }
   
   qlwi = mpQueue->item(0);
   if( qlwi )
   {
      selected = qlwi->isSelected();
      qlwi = mpQueue->takeItem(0);
      delete qlwi;
   }

   if ( mAborting )
   {
      if ( mpFile )
      {
         mpFile->close();
         mpFile->remove();
         delete mpFile;
         mpFile = 0;
      }
      error = true;
   }

   if( mpFile )
   {
      mpFile->close();
      
      if( error )
      {
         mpFile->remove();
         emit errorMessage( mURL + tr(" Download failed:") +
                            mpHttp->errorString() );
      }
      
      delete mpFile;
      mpFile = 0;
   }

   mPostDownloadHandler->run( mURL, mFileName, !error, selected );
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
      run( responseHeader.value("location"), mFileName, mPostDownloadHandler, mpQueue->item(0)->isSelected() );
   }
   
   if( responseHeader.statusCode() != 200 )
   {
      errorMessage( mURL+QString(":")+QString::number( responseHeader.statusCode() )+
                    QString(" ")+responseHeader.reasonPhrase() );
      mAborting = true;
   }
}


void DownloadHandler::run( const QString &url,
                           const QString &filename,
                           PostDownloadHandler *postDownloadHandler,
                           bool selected )
{
   if( !url.startsWith("http://") )
   {
      return;
   }
   
   mURLs.append( url );
   mFileNames.append( filename );
   mPostDownloadHandlers.append( postDownloadHandler );
   
   mpQueue->addItem( filename + QString( " <- " ) + url );
   QListWidgetItem *qlwi = mpQueue->item( mpQueue->count() - 1 );
   if( qlwi )
   {
      qlwi->setSelected( selected );
   }
}


void DownloadHandler::startDownload()
{
   if( mpFile )
   {
      return;
   }
   if( mURLs.size() == 0 )
   {
      emit downloadActive( false );
      mpProgressBar->setValue(0);
      return;
   }
   emit downloadActive( true );
   
   mPostDownloadHandler = mPostDownloadHandlers.takeFirst();
   mURL                 = mURLs.takeFirst();
   mFileName            = mFileNames.takeFirst();
   mDownloading         = true;
   mAborting            = false;
   
   mpFile = new QFile( mFileName );
   if ( !mpFile->open( QIODevice::WriteOnly ) )
   {
      delete mpFile;
      mpFile = 0;
      return;
   }

   QString url( mURL );
   if( url.startsWith( "http://" ) )
   {
      url = url.mid( 7 );
   }
   int slash1 = url.indexOf('/');
   
   ProxyWidget::setProxy( mpHttp );
   mpHttp->setHost( url.left(slash1) );
   mHttpGetId = mpHttp->get( url.mid(slash1), mpFile );
}
