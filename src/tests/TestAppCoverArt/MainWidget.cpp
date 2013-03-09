/*
 * src/tests/TestAppWebServer/MainWidget.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

/* class declaration */
#include "MainWidget.hpp"

/* system headers */

/* Qt headers */
#include <QtGui>
#include <QByteArray>
#include <QFileSystemModel>
#include <QFileSystemWatcher>
#include <QList>
#include <QModelIndex>
#include <QRegExp>
#include <QSignalMapper>
#include <QSplitter>
#include <QTreeView>
#include <QUrl>
#include <QXmlStreamReader>

/* local library headers */
#include <Database.hpp>
#include <ImageWidget.hpp>
#include <ProxyWidget.hpp>
#include <Satellite.hpp>
#include <Settings.hpp>

/* local headers */
#include <Kryptonite.hpp>
#include <KryptoniteJobCoverAmazonDE.hpp>
#include <KryptoniteJobCoverDiscogs.hpp>
#include "DropImageWidget.hpp"

#include "Trace.hpp"

MainWidget::MainWidget( QWidget *parent )
: QSplitter( Qt::Horizontal, parent )
, mpKryptonite( new Kryptonite() )
, mpAmazonDE( new KryptoniteJobCoverAmazonDE( mpKryptonite ) )
, mpDiscogs( new KryptoniteJobCoverDiscogs( mpKryptonite ) )
, mpLayout( new QGridLayout( this ) )
, mpFileSysTree( new QTreeView( this ) )
, mpFileSysModel( new QFileSystemModel( this ) )
, mpLineEdit( new QLineEdit( this ) )
, mpFollowPartyman( new QCheckBox( tr("Follow Partyman"), this ) )
, mpCopyBuffer( new QPushButton( tr("Copy debug buffer to clipboard"), this ) )
, mpImage( new DropImageWidget( this ) )
, mpInfo( new QListWidget( this ) )
, mpSignalMapper( new QSignalMapper( this ) )
, mDataMap()
, mCacheMap()
, mDebugData()
{
TRACESTART(MainWidget::MainWidget)
   mpKryptonite->setObjectName( "Downloader");
   mpAmazonDE->setObjectName( "Amazon" );
   mpDiscogs->setObjectName( "Discogs" );
   mpFileSysTree->setObjectName( "FileSysTree" );
   mpFileSysModel->setObjectName( "FileSysModel" );
   mpLineEdit->setObjectName( "LineInput" );
   mpFollowPartyman->setObjectName( "FollowPartyman" );
   mpFollowPartyman->setChecked( true );
   mpCopyBuffer->setObjectName( "CopyBuffer" );
   mpImage->setObjectName( "Image" );
   mpInfo->setObjectName( "Info" );
   QThread *t = new QThread();
   connect( qApp, SIGNAL(aboutToQuit()),
            t, SLOT(quit()) );
   ProxyWidget::setProxy( mpKryptonite->networkAccessManager() );
   mpKryptonite->moveToThread( t );
   mpAmazonDE->moveToThread( t );
   mpDiscogs->moveToThread( t );
   t->setObjectName( "DownloadThread" );
   t->start();
   QWidget *w = 0;
   mpFileSysTree->setModel( mpFileSysModel );
   mpFileSysModel->setRootPath( "/" );
   mpFileSysModel->setFilter( QDir::NoDotAndDotDot | QDir::AllDirs );
   const QString current( Settings::value( Settings::RubberbandmanRootDirectory ) );
   QModelIndex qmi( mpFileSysModel->index( current ) );
   if( qmi.isValid() )
   {
      mpFileSysTree->setRootIndex( qmi );
      mpFileSysTree->setCurrentIndex( mpFileSysModel->index( current ) );
   }
   mpFileSysTree->header()->hide();
   mpFileSysTree->setColumnHidden( 1, true );
   mpFileSysTree->setColumnHidden( 2, true );
   mpFileSysTree->setColumnHidden( 3, true );

   QSplitter *s = new QSplitter( Qt::Vertical, this );
   w = new QWidget( this );
   QVBoxLayout *v = new QVBoxLayout( w );
   v->addWidget( mpFileSysTree );
   v->addWidget( mpLineEdit );
   QHBoxLayout *h = new QHBoxLayout();
   v->addLayout( h );
   h->addWidget( mpFollowPartyman );
   h->addWidget( mpCopyBuffer );
   s->addWidget( w );
   w = new QWidget( this );
   w->setLayout( mpLayout );
   s->addWidget( w );
   addWidget( s );
   w = new QWidget( this );
   v = new QVBoxLayout( w );
   v->addWidget( mpImage );
   v->addWidget( mpInfo );
   addWidget( w );
   v->setStretch( 0, 1 );
   Satellite *satellite = Satellite::get();

   connect( mpImage, SIGNAL(droppedUrl(QUrl)),
            this, SLOT(saveImage(QUrl)) );
   connect( mpCopyBuffer, SIGNAL(clicked()),
            this, SLOT(debugBufferToClipboard()) );
   connect( mpLineEdit, SIGNAL(returnPressed()),
            this, SLOT(requestFromLine()) );
   connect( mpFileSysTree, SIGNAL(clicked(QModelIndex)),
            this, SLOT(entryClicked(QModelIndex)) );
   connect( mpSignalMapper, SIGNAL(mapped(QWidget*)),
            this, SLOT(saveImage(QWidget*)) );
   connect( this, SIGNAL(requestSearch(QString)),
            mpDiscogs, SLOT(requestList(QString)) );
   connect( mpDiscogs, SIGNAL(imageFound(QByteArray,QVariant)),
            this, SLOT(addThumbnail(QByteArray,QVariant)) );
   connect( mpDiscogs, SIGNAL(imageDownloaded(QByteArray,QVariant)),
            this, SLOT(showImage(QByteArray,QVariant)) );
   connect( mpDiscogs, SIGNAL(message(QString,QByteArray)),
            this, SLOT(message(QString,QByteArray)) );
   connect( this, SIGNAL(requestSearch(QString)),
            mpAmazonDE, SLOT(requestList(QString)) );
   connect( mpAmazonDE, SIGNAL(imageFound(QByteArray,QVariant)),
            this, SLOT(addThumbnail(QByteArray,QVariant)) );
   connect( mpAmazonDE, SIGNAL(imageDownloaded(QByteArray,QVariant)),
            this, SLOT(showImage(QByteArray,QVariant)) );
   connect( mpAmazonDE, SIGNAL(message(QString,QByteArray)),
            this, SLOT(message(QString,QByteArray)) );
   connect( satellite, SIGNAL(received(QByteArray)),
            this, SLOT(handleSatelliteMessage(QByteArray)) );

   QList<int> sizes;
   sizes << 30 << 300;
   s->setSizes( sizes );
}


MainWidget::~MainWidget()
{
}


void MainWidget::handleSatelliteMessage( const QByteArray &msg )
{
   QStringList src( Satellite::split( msg ) );
   if( src.size() > 0 )
   {
      if( src.at(0) == "p0p" )
      {
         if( mpFollowPartyman->isChecked() && (src.size() > 1)  )
         {
            QFileInfo info( src.at(1) );
            QString dirName( info.absolutePath() );
            if( info.exists() )
            {
               QModelIndex qmi;
               for( int i = 1; i >= 0; i = dirName.indexOf( "/", i + 1 ) )
               {
                  qmi = mpFileSysModel->index( dirName.left( i ) );
                  mpFileSysTree->expand( qmi );
               }
               qmi = mpFileSysModel->index( dirName );
               mpFileSysTree->setCurrentIndex( qmi );
               entryClicked( qmi );
            }
         }
      }
   }
}


void MainWidget::message( const QString &message, const QByteArray &data )
{
   if( !data.isNull() )
   {
      mDebugData = data;
   }
   mpInfo->addItem( message );

   // ugly hack, but works as intended:
   while( mpInfo->verticalScrollBar()->isVisible() )
   {
      delete mpInfo->takeItem( 0 );
      QApplication::processEvents();
   }
   mpInfo->scrollToBottom();
}


void MainWidget::debugBufferToClipboard()
{
   QApplication::clipboard()->setText( QString::fromUtf8( mDebugData.constData(), mDebugData.size() ) );
}


void MainWidget::entryClicked( const QModelIndex &index )
{
TRACESTART(MainWidget::entryClicked)
   QString dirName( mpFileSysModel->filePath(index) );
   QRegExp re( ".*/([^/]*)/([^/]*)" );
   QDir dir( dirName );
   dirName.replace( re, "\\1 \\2" );
   mpLineEdit->setText( dirName );
   requestFromLine();
   QString searchPattern( Settings::value( Settings::FunkytownCoverFile ) );
   searchPattern.append( ".*" );
   QStringList entries( dir.entryList( QStringList( searchPattern ) ) );
   if( entries.size() > 0 )
   {
      mpImage->setImage( dir.absoluteFilePath( entries.at(0) ) );
      QImage i( mpImage->imageData() );
      message( tr("%1 x %2: %3").arg( QString::number(i.width()),
                                      QString::number(i.height()),
                                      dir.absoluteFilePath( entries.at(0) ) ) );
   }
   else
   {
      mpImage->setImage( QImage() );
   }
}


void MainWidget::requestFromLine()
{
TRACESTART(MainWidget::requestFromLine)
   foreach( QWidget *w, mDataMap.keys() )
   {
      delete w;
   }
   mDataMap.clear();
   mCacheMap.clear();
   mDebugData.clear();
   mpInfo->clear();
   emit requestSearch( mpLineEdit->text() );
}


void MainWidget::addThumbnail( const QByteArray &data, const QVariant &payload )
{
TRACESTART(MainWidget::addThumbnail)
   QUrl url( payload.toUrl() );
   QImage image( QImage::fromData( data ) );
   ImageWidget *w = new ImageWidget( this );
   w->setImage( image );
   w->setToolTip( tr("%1 x %2").arg( QString::number(image.width()),
                                     QString::number(image.height())) );
   int x    = 0;
   int y    = 0;
   int i = mDataMap.size();
   int sqr  = (int)sqrt( i );
   int dist = i - (sqr * sqr);
   if( dist < sqr )
   {
      x = sqr;
      y = dist;
   }
   else
   {
      x = dist - sqr;
      y = sqr;
   }
   mpLayout->addWidget( w, y, x );
   mDataMap.insert( w, url );
   connect( w, SIGNAL(clicked(QPoint)),
            mpSignalMapper, SLOT(map()) );
   mpSignalMapper->setMapping( w, w );
}


void MainWidget::saveImage( const QUrl &url )
{
   message( tr( "Image dropped: %1" ).arg(url.toString()) );
   QModelIndex index( mpFileSysTree->currentIndex() );
   QDir dir( mpFileSysModel->filePath(index) );
   QString fileName( dir.absoluteFilePath( Settings::value( Settings::FunkytownCoverFile ) ) );
   message( url.toString() );
   QStringList payload;
   QString scheme( url.scheme().toLower() );
   if( scheme.startsWith("http") )
   {
      payload << url.toString() << fileName;
      mpKryptonite->download( this, SLOT(showImage(QByteArray,QVariant)), url, payload );
   }
   else if( scheme == "file" )
   {
      payload << QString() << fileName;
      QFile f( url.toLocalFile() );
      if( f.open( QIODevice::ReadOnly ) )
      {
         showImage( f.readAll(), payload );
         f.close();
      }
   }
}


void MainWidget::saveImage( QWidget *widget )
{
   QModelIndex index( mpFileSysTree->currentIndex() );
   QDir dir( mpFileSysModel->filePath(index) );
   QString fileName( dir.absoluteFilePath( Settings::value( Settings::FunkytownCoverFile ) ) );
   message( mDataMap.value( widget ).toString() );
   QString url( mDataMap.value( widget ).toString() );
   QStringList payload;
   payload << url << fileName;
   if( mCacheMap.contains( url ) )
   {
      message( tr( "Using cached image" ) );
      showImage( mCacheMap.value( url ), payload );
   }
   else
   {
      if( url.startsWith("http://www.amazon.de/") )
      {
         KryptoniteJobCover::requestImage( mpAmazonDE, mDataMap.value( widget ), payload );
      }
      else if( url.startsWith("http://www.discogs.com/") )
      {
         KryptoniteJobCover::requestImage( mpDiscogs, mDataMap.value( widget ), payload );
      }
   }
//   emit requestItem( mDataMap.value( widget ) );
}


void MainWidget::showImage( const QByteArray &data, const QVariant &payload )
{
TRACESTART(MainWidget::showImage)
TRACEMSG << payload.toString();
   mpImage->setImage( data );
   QStringList parameters( payload.toStringList() );
   QString url( parameters.takeFirst() );
   QString fileName( parameters.takeFirst() );
   if( !url.isEmpty() && !mCacheMap.contains( url ) )
   {
      mCacheMap.insert( url, data );
   }
   fileName.append( "." );
   if( !Settings::value( Settings::FunkytownCoverConvertTo ).isEmpty() )
   {
      fileName.append( Settings::value( Settings::FunkytownCoverConvertTo ) );
      QImage::fromData( data ).save( fileName );
   }
   else
   {
      /* detect file format from binary data for file extension */
      QByteArray dummy( data );
      QBuffer buffer( &dummy );
      buffer.open( QIODevice::ReadOnly );
      QByteArray extension( QImageReader::imageFormat( &buffer ).toLower() );
      extension.replace( "jpeg", "jpg" );
      fileName.append( extension.constData() );
      QFile f( fileName );
      if( f.open( QIODevice::WriteOnly ) )
      {
         f.write( data );
         f.close();
      }
   }
   QImage i( mpImage->imageData() );
   message( tr("%1 x %2: %3").arg( QString::number(i.width()),
                                   QString::number(i.height()),
                                   fileName ));
}
