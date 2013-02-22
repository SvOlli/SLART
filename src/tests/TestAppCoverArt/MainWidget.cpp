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
#include <Settings.hpp>

/* local headers */
#include "Downloader.hpp"
#include "DownloadJobCoverAmazonDE.hpp"

#include "Trace.hpp"

MainWidget::MainWidget( QWidget *parent )
: QSplitter( Qt::Horizontal, parent )
, mpDownloader( new Downloader() )
, mpAmazon( new DownloadJobCoverAmazonDE( mpDownloader, this, SLOT(showCovers(QByteArray,QVariant)) ) )
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
TRACEMSG << QMetaObject::normalizedSignature(SLOT(showCovers(QByteArray,QVariant)))
         << QMetaObject::checkConnectArgs(SLOT(showCovers(QByteArray,QVariant)),SLOT(x(QByteArray)));
   QThread *t = new QThread();
   connect( qApp, SIGNAL(aboutToQuit()),
            t, SLOT(quit()) );
   mpDownloader->moveToThread( t );
   mpAmazon->moveToThread( t );
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
   mpAmazon->query( mpLineEdit->text() );
}


void MainWidget::showCovers( const QByteArray &data, const QVariant &payload )
{
TRACESTART(MainWidget::showCovers)
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
   ImageWidget *i = qobject_cast<ImageWidget*>(widget);
   const QImage &img = i->imageData();

   QString fileName("/tmp/AlbumArt.png");
   img.save( fileName );
   mpImage->setImage( fileName );
   mpMessage->setText( mDataMap.value( widget ).toString() );
}
