/**
 * DownloadHandler.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#include "DownloadHandler.hpp"
#include "PostDownloadHandler.hpp"

#include <QtGui>
#include <QtNetwork>

#include "Trace.hpp"

DownloadHandler::DownloadHandler( QWidget *parent )
: QWidget( parent )
, mDownloading( false )
, mHttpGetId( 0 )
, mpHttp( new QHttp( this ) )
, mpFile( 0 )
, mpLayout( new QVBoxLayout )
, mpProgressBar( new QProgressBar )
, mpQueue( new QListWidget )
, mpTimer( new QTimer(this) )
{
 #if QT_VERSION < 0x040300
   mpLayout->setMargin( 0 );
#else
   mpLayout->setContentsMargins( 0, 0, 0, 0 );
#endif
   mpLayout->addWidget( mpProgressBar );
   mpLayout->addWidget( mpQueue );
   setLayout( mpLayout );

   connect(mpHttp, SIGNAL(requestFinished(int, bool)),
           this, SLOT(httpRequestFinished(int, bool)));
   connect(mpHttp, SIGNAL(dataReadProgress(int, int)),
           this, SLOT(updateDataReadProgress(int, int)));
   connect(mpHttp, SIGNAL(responseHeaderReceived(const QHttpResponseHeader &)),
           this, SLOT(readResponseHeader(const QHttpResponseHeader &)));
   connect(mpTimer, SIGNAL(timeout()), this, SLOT(startDownload()));

   mpTimer->start(1000);
}


DownloadHandler::~DownloadHandler()
{
}


void DownloadHandler::httpRequestFinished( int requestId, bool error )
{
   QListWidgetItem *qlwi;
   
   if ( requestId != mHttpGetId )
   {
      return;
   }
   
   qlwi = mpQueue->takeItem(0);
   if( qlwi )
   {
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
         emit errorMessage( mURL+QString( tr(" Download failed:") )+
                            mpHttp->errorString() );
      }
      
      delete mpFile;
      mpFile = 0;
   }

   mPostDownloadHandler->run( mURL, mFileName, !error );
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
   if (responseHeader.statusCode() != 200)
   {
      errorMessage( mURL+QString(":")+QString::number( responseHeader.statusCode() )+
                    QString(" ")+responseHeader.reasonPhrase() );
      mAborting = true;
   }
}


void DownloadHandler::run( const QString &url,
                           const QString &filename,
                           PostDownloadHandler *postDownloadHandler )
{
   if( url.startsWith("http://") )
   {
      mURLs.append( url );
   }
   else
   {
      mURLs.append( QString("http://www.myspace.com/")+url );
   }
   mFileNames.append( filename );
   mPostDownloadHandlers.append( postDownloadHandler );
   mpQueue->addItem( filename + QString( " <- " ) + url );
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
   QString url          = mURL;
   mDownloading         = true;
   mAborting            = false;
   
   mpFile = new QFile( mFileName );
   if ( !mpFile->open( QIODevice::WriteOnly ) )
   {
      delete mpFile;
      mpFile = 0;
      return;
   }
   
   if( url.startsWith( "http://" ) )
   {
      url = url.mid( 7 );
   }
   int slash1 = url.indexOf('/');
   
   mpHttp->setHost( url.left(slash1) );
   mHttpGetId = mpHttp->get( url.mid(slash1), mpFile );
}


void DownloadHandler::setProxy( ProxyWidget *proxyWidget )
{
   proxyWidget->setProxy( mpHttp );
}
