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
#include <Settings.hpp>

/* local headers */
#include <Kryptonite.hpp>
#include <KryptoniteJobCoverAmazonDE.hpp>

#include "Trace.hpp"

MainWidget::MainWidget( QWidget *parent )
: QSplitter( Qt::Horizontal, parent )
, mpKryptonite( new Kryptonite() )
, mpAmazon( new KryptoniteJobCoverAmazonDE( mpKryptonite ) )
, mpLayout( new QGridLayout( this ) )
, mpFileSysTree( new QTreeView( this ) )
, mpFileSysModel( new QFileSystemModel( this ) )
, mpLineEdit( new QLineEdit( this ) )
, mpImage( new ImageWidget( this ) )
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
   mpAmazon->moveToThread( t );
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
   s->addWidget( w );
   w = new QWidget( this );
   w->setLayout( mpLayout );
   s->addWidget( w );
   addWidget( s );
   w = new QWidget( this );
   v = new QVBoxLayout( w );
   v->addWidget( mpImage );
   v->addWidget( mpMessage );
   addWidget( w );
   QFileSystemWatcher *watcher = new QFileSystemWatcher( this );
   watcher->addPath( "/tmp" );

   connect( watcher, SIGNAL(fileChanged(QString)),
            mpImage, SLOT(setImage(QString)) );

   connect( mpLineEdit, SIGNAL(returnPressed()),
            this, SLOT(requestFromLine()) );
   connect( mpFileSysTree, SIGNAL(clicked(QModelIndex)),
            this, SLOT(entryClicked(QModelIndex)) );
   connect( mpSignalMapper, SIGNAL(mapped(QWidget*)),
            this, SLOT(saveImage(QWidget*)) );
   connect( this, SIGNAL(requestSearch(QString)),
            mpAmazon, SLOT(requestList(QString)) );
   connect( mpAmazon, SIGNAL(imageFound(QByteArray,QVariant)),
            this, SLOT(addThumbnail(QByteArray,QVariant)) );
   connect( mpAmazon, SIGNAL(imageDownloaded(QByteArray,QVariant)),
            this, SLOT(showImage(QByteArray,QVariant)) );

   QList<int> sizes;
   sizes << 30 << 300;
   s->setSizes( sizes );
}


MainWidget::~MainWidget()
{
}


void MainWidget::entryClicked( const QModelIndex &index )
{
TRACESTART(MainWidget::entryClicked)
   QString dirName( mpFileSysModel->filePath(index) );
   QRegExp re( ".*/([^/]*)/([^/]*)" );
   dirName.replace( re, "\\1 \\2" );
   mpLineEdit->setText( dirName );
   requestFromLine();
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
   mpLayout->addWidget( w, mDataMap.size() / mNumColumns, mDataMap.size() % mNumColumns );
   mDataMap.insert( w, url );
   connect( w, SIGNAL(clicked(QPoint)),
            mpSignalMapper, SLOT(map()) );
   mpSignalMapper->setMapping( w, w );
}


void MainWidget::saveImage( QWidget *widget )
{
   QModelIndex index( mpFileSysTree->currentIndex() );
   QDir dir( mpFileSysModel->filePath(index) );
   QString fileName( dir.absoluteFilePath( "AlbumArt.%1" ) );
   mpMessage->setText( mDataMap.value( widget ).toString() );
   KryptoniteJobCover::requestImage( mpAmazon, mDataMap.value( widget ), fileName );
//   emit requestItem( mDataMap.value( widget ) );
}


void MainWidget::showImage( const QByteArray &data, const QVariant &payload )
{
TRACESTART(MainWidget::showImage)
TRACEMSG << payload.toString();
   mpImage->setImage( data );
   QString fileName( payload.toString() );
   if( fileName.contains("%1") )
   {
      QByteArray dummy( data );
      QBuffer buffer( &dummy );
      buffer.open( QIODevice::ReadOnly );
      QByteArray extension( QImageReader::imageFormat( &buffer ).toLower() );
      extension.replace( "jpeg", "jpg" );
      fileName = fileName.arg( extension.constData() );
   }
   QFile f( fileName );
   if( f.open( QIODevice::WriteOnly ) )
   {
      f.write( data );
      f.close();
      mpMessage->setText( f.fileName() );
   }
}
