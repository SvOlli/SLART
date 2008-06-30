/**
 * FileSysBrowser.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#include "FileSysBrowser.hpp"
#include "MySettings.hpp"
#include "Database.hpp"

#include <QtGui>
#include <QString>

#include "Trace.hpp"


FileSysBrowser::FileSysBrowser( Database *database, QWidget *parent, Qt::WindowFlags flags )
: QWidget( parent, flags )
, mpDatabase( database )
, mpRootDir( new QLineEdit( this ) )
, mpSetButton( new QPushButton( tr("Set"), this ) )
, mpDotButton( new QPushButton( tr(".."), this ) )
, mpTimer( new QTimer( this ) )
, mpView( new QTreeView( this ) )
, mpModel( new QDirModel( this ) )
{
   MySettings settings;
   
   QStringList filterNames;
   filterNames << "*.mp2" << "*.mp3" << "*.ogg";
   mpModel->setNameFilters( filterNames );
   mpModel->setFilter( QDir::NoDotAndDotDot | QDir::AllDirs | QDir::Files );
   mpModel->setSorting( QDir::Name | QDir::DirsFirst | QDir::IgnoreCase /*| QDir::LocaleAware*/ );
   mpModel->setLazyChildCount( true );
   
   mpView->setContextMenuPolicy( Qt::CustomContextMenu );
   mpView->setModel( mpModel );
   
//   connect(mpView, SIGNAL(context(QModelIndex)), this, SLOT(addEntries(QModelIndex)));
   QVBoxLayout *layout = new QVBoxLayout( this );
   QHBoxLayout *topLayout = new QHBoxLayout;
   
   topLayout->addWidget( new QLabel(tr("Root:")) );
   topLayout->addWidget( mpRootDir );
   topLayout->addWidget( mpSetButton );
   topLayout->addWidget( mpDotButton );
#if QT_VERSION < 0x040300
   topLayout->setMargin( 0 );
#else
   topLayout->setContentsMargins( 0, 0, 0, 0 );
#endif
   
   layout->addLayout( topLayout );
   layout->addWidget( mpView );
#if QT_VERSION < 0x040300
   layout->setMargin( 2 );
#else
   layout->setContentsMargins( 2, 2, 2, 2 );
#endif
   setLayout(layout);
   
   mpTimer->setSingleShot( true );
   mpTimer->setInterval( 2500 );
   
   /* evil hack */
   mpSetButton->setMaximumWidth( mpSetButton->width() / 2 );
   mpDotButton->setMaximumWidth( mpDotButton->width() / 2 );
   
   mpRootDir->setText( settings.value( "RootDirectory", QString("/") ).toString() );
   //mpView->setRootIndex( mpModel->index( mpRootDir->text() ) );
   handleRootDir();
   mpView->setAnimated( true );
   connect( mpView, SIGNAL(clicked(const QModelIndex&)),
            this, SLOT(entryClicked(const QModelIndex&)) );
   connect( mpRootDir, SIGNAL(returnPressed()),
            this, SLOT(handleRootDir()) );
   connect( mpSetButton, SIGNAL(clicked()),
            this, SLOT(handleRootDir()) );
   connect( mpDotButton, SIGNAL(clicked()),
            this, SLOT(handleDotButton()) );
   connect( mpTimer, SIGNAL(timeout()),
            this, SLOT(handleTimer()) );
   connect( mpView, SIGNAL(customContextMenuRequested(const QPoint&)),
            this, SLOT(contextMenu(const QPoint&)) );
}


void FileSysBrowser::entryClicked( const QModelIndex &index )
{
   QString path( mpModel->filePath(index) );
   
   if( QFileInfo( path ).isDir() )
   {
      mpRootDir->setText( path );
      mpTimer->start();
   }
   emit clicked( path );
}


void FileSysBrowser::handleDotButton()
{
TRACESTART(FileSysBrowser::handleDotButton)
   QDir dir( mpRootDir->text() + "/.." );
   mpRootDir->setText( dir.absolutePath() );
   handleRootDir();
}


void FileSysBrowser::handleRootDir()
{
   QModelIndex qmi( mpModel->index( mpRootDir->text() ) );
   if( qmi.isValid() )
   {
      QString current( mpModel->filePath( mpView->currentIndex() ) );
      MySettings().setValue( "RootDirectory", mpRootDir->text() );
      mpModel->refresh( qmi );
      mpView->setRootIndex( qmi );
      mpView->setCurrentIndex( mpModel->index( current ) );
      scrollTo( current );
   }
   else
   {
      mpTimer->start();
   }
   mpView->header()->hide();
   mpView->setColumnHidden( 1, true );
   mpView->setColumnHidden( 2, true );
   mpView->setColumnHidden( 3, true );
}


void FileSysBrowser::handleTimer()
{
   mpRootDir->setText( MySettings().value( "RootDirectory", QString("/") ).toString() );
}


void FileSysBrowser::scrollTo( const QString &fileName )
{
   QModelIndex qmi( mpModel->index( fileName ) );
   mpView->scrollTo( qmi, QAbstractItemView::PositionAtCenter );
   mpView->setCurrentIndex( qmi );
   /* TODO: only emit if in visible area */
   emit clicked( fileName );
}


void FileSysBrowser::contextMenu( const QPoint &pos )
{
   QModelIndex qmi( mpView->indexAt( pos ) );
   if( !mpModel->isDir( qmi ) )
   {
      QString msg( mpModel->filePath( qmi ) );
      msg.prepend( "P0Q\n" );
      MySettings().sendUdpMessage( msg, "Partyman" );
   }
}
