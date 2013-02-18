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
: QSplitter( Qt::Vertical, parent )
, mpDownloader( new Downloader() )
, mpAmazon( new DownloadJobCoverAmazonDE( mpDownloader, this, SLOT(showCovers(QByteArray)) ) )
, mpLayout( new QGridLayout( this ) )
, mpFileSysTree( new QTreeView( this ) )
, mpFileSysModel( new QFileSystemModel( this ) )
, mpLineEdit( new QLineEdit( this ) )
, mpSignalMapper( new QSignalMapper( this ) )
, mCoversList()
, mDataMap()
{
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

   w = new QWidget( this );
   QVBoxLayout *v = new QVBoxLayout( w );
   v->addWidget( mpFileSysTree );
   v->addWidget( mpLineEdit );
   addWidget( w );
   w = new QWidget( this );
   w->setLayout( mpLayout );
   addWidget( w );

   connect( mpLineEdit, SIGNAL(returnPressed()),
            this, SLOT(requestFromLine()) );
   connect( mpFileSysTree, SIGNAL(clicked(QModelIndex)),
            this, SLOT(entryClicked(QModelIndex)) );
   connect( mpSignalMapper, SIGNAL(mapped(QWidget*)),
            this, SLOT(saveImage(QWidget*)) );

   QList<int> sizes;
   sizes << 30 << 300;
   setSizes( sizes );
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
   while( !mCoversList.isEmpty() )
   {
      delete mCoversList.takeLast();
   }
   mDataMap.clear();
   mpAmazon->query( mpLineEdit->text() );
}


void MainWidget::showCovers( const QByteArray &data )
{
TRACESTART(MainWidget::showCovers)
   QImage image( QImage::fromData( data ) );
   ImageWidget *w = new ImageWidget( this );
   w->setImage( image );
   w->setToolTip( tr("%1 x %2").arg( QString::number(image.width()),
                                      QString::number(image.height())) );
TRACEMSG << image.size() << mCoversList.size() / 5 << mCoversList.size() % 5;
   mpLayout->addWidget( w, mCoversList.size() / 5, mCoversList.size() % 5 );
   mCoversList.append( w );
   mDataMap.insert( w, data );
   connect( w, SIGNAL(clicked(QPoint)),
            mpSignalMapper, SLOT(map()) );
   mpSignalMapper->setMapping( w, w );
}


void MainWidget::saveImage( QWidget *widget )
{
   QString fileName("/tmp/AlbumArt");
   QFile f( fileName );
   if( f.open( QIODevice::WriteOnly ) )
   {
      f.write( mDataMap.value( widget ) );
      f.close();
      QString extension( QString::fromLocal8Bit( QImageReader::imageFormat( fileName ).constData() ) );
      extension.replace( "jpeg", "jpg" );
      fileName = QString("%1.%2").arg( fileName, extension );
      QFile::remove( fileName );
      f.rename( fileName );
   }
}
