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
, mpDownloader( new Downloader( this ) )
, mpAmazon( new DownloadJobCoverAmazonDE( mpDownloader, this ) )
, mpLayout( new QGridLayout( this ) )
, mpFileSysTree( new QTreeView( this ) )
, mpFileSysModel( new QFileSystemModel( this ) )
, mpLineEdit( new QLineEdit( this ) )
, mpListWidget( 0 )
, mCoversList()
{
   QWidget *w = 0;
   mpAmazon->setTarget( this, SLOT(showCovers(QList<QByteArray>)) );
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

   QList<int> sizes;
   sizes << 30 << 300;
   setSizes( sizes );
}


MainWidget::~MainWidget()
{
}

void MainWidget::requestFromLine()
{
TRACESTART(MainWidget::requestFromLine)
   mpAmazon->query( mpLineEdit->text() );
}


void MainWidget::showCovers( const QList<QByteArray> &data )
{
TRACESTART(MainWidget::showCovers)
   while( !mCoversList.isEmpty() )
   {
      delete mCoversList.takeLast();
   }
   QImage image;
   for( int i = 0; i < data.size(); ++i )
   {
      image = QImage::fromData( data.at(i) );
      ImageWidget *w = new ImageWidget( this );
      w->setImage( image );
      w->setToolTip( tr("%1 x %2").arg( QString::number(image.width()),
                                      QString::number(image.height())) );
TRACEMSG << image.size() << i/5 << i % 5;
      mpLayout->addWidget( w, i/5, i % 5 );
      mCoversList.append( w );
   }
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
