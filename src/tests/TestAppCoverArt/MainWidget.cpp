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
, mpImage( new ImageWidget( this ) )
, mpInfo( new QLabel( this ) )
, mpMessage( new QLineEdit( this ) )
, mpSignalMapper( new QSignalMapper( this ) )
, mDataMap()
, mNumColumns( 4 )
{
TRACESTART(MainWidget::MainWidget)
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
   mpInfo->setAlignment( Qt::AlignCenter );

   QSplitter *s = new QSplitter( Qt::Vertical, this );
   w = new QWidget( this );
   QVBoxLayout *v = new QVBoxLayout( w );
   v->addWidget( mpFileSysTree );
   v->addWidget( mpLineEdit );
   v->addWidget( mpFollowPartyman );
   s->addWidget( w );
   w = new QWidget( this );
   w->setLayout( mpLayout );
   s->addWidget( w );
   addWidget( s );
   w = new QWidget( this );
   v = new QVBoxLayout( w );
   v->addWidget( mpImage );
   v->addWidget( mpInfo );
   v->addWidget( mpMessage );
   addWidget( w );
   v->setStretch( 0, 1 );
   QFileSystemWatcher *watcher = new QFileSystemWatcher( this );
   watcher->addPath( "/tmp" );
   Satellite *satellite = Satellite::get();

   connect( watcher, SIGNAL(fileChanged(QString)),
            mpImage, SLOT(setImage(QString)) );

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
   connect( this, SIGNAL(requestSearch(QString)),
            mpAmazonDE, SLOT(requestList(QString)) );
   connect( mpAmazonDE, SIGNAL(imageFound(QByteArray,QVariant)),
            this, SLOT(addThumbnail(QByteArray,QVariant)) );
   connect( mpAmazonDE, SIGNAL(imageDownloaded(QByteArray,QVariant)),
            this, SLOT(showImage(QByteArray,QVariant)) );
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
      mpMessage->setText( dir.absoluteFilePath( entries.at(0) ) );
      QImage i( mpImage->imageData() );
      mpInfo->setText( tr("%1 x %2").arg( QString::number(i.width()), QString::number(i.height())) );
   }
   else
   {
      mpImage->setImage( QImage() );
      mpInfo->clear();
      mpMessage->clear();
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
   int i = mDataMap.size();
   int sq = (int)sqrt( i );
   int sd = i - (sq * sq);
   int x = 0;
   int y = 0;
   if( sd >= sq )
   {
      x = sd - sq;
      y = sq;
   }
   else
   {
      x = sq;
      y = sd;
   }
   mpLayout->addWidget( w, y, x );
   mDataMap.insert( w, url );
   connect( w, SIGNAL(clicked(QPoint)),
            mpSignalMapper, SLOT(map()) );
   mpSignalMapper->setMapping( w, w );
}


void MainWidget::saveImage( QWidget *widget )
{
   QModelIndex index( mpFileSysTree->currentIndex() );
   QDir dir( mpFileSysModel->filePath(index) );
   QString fileName( dir.absoluteFilePath( Settings::value( Settings::FunkytownCoverFile ) ) );
   mpMessage->setText( mDataMap.value( widget ).toString() );
   QString url( mDataMap.value( widget ).toString() );
   if( url.startsWith("http://www.amazon.de/") )
   {
      KryptoniteJobCover::requestImage( mpAmazonDE, mDataMap.value( widget ), fileName );
   }
   else if( url.startsWith("http://www.discogs.com/") )
   {
      KryptoniteJobCover::requestImage( mpDiscogs, mDataMap.value( widget ), fileName );
   }
//   emit requestItem( mDataMap.value( widget ) );
}


void MainWidget::showImage( const QByteArray &data, const QVariant &payload )
{
TRACESTART(MainWidget::showImage)
TRACEMSG << payload.toString();
   mpImage->setImage( data );
   QString fileName( payload.toString() );
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
   mpMessage->setText( fileName );
   QImage i( mpImage->imageData() );
   mpInfo->setText( tr("%1 x %2").arg( QString::number(i.width()), QString::number(i.height())) );
}
