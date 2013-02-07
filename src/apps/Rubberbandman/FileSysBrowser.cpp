/*
 * src/apps/Rubberbandman/FileSysBrowser.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "FileSysBrowser.hpp"

/* system headers */

/* Qt headers */
#include <QtGui>
#include <QString>

/* local library headers */
#include <DatabaseInterface.hpp>
#include <DirWalker.hpp>
#include <Settings.hpp>
#include <Satellite.hpp>

/* local headers */
#include "DirWalkerDelete.hpp"
#include "DirWalkerMove.hpp"
#include "MyTreeView.hpp"


FileSysBrowser::FileSysBrowser( QWidget *parent )
: QWidget( parent )
, mpDatabase( DatabaseInterface::get() )
, mpRootDir( new QLineEdit( this ) )
, mpDotButton( new QPushButton( "...", this ) )
, mpView( new MyTreeView( this ) )
, mpModel( new QDirModel( this ) )
, mpMenuSendToPartyman( new QAction( tr("Send To Partyman"), this ) )
, mpMenuRescan( new QAction( tr("Rescan"), this ) )
, mpMenuMoveFile( new QAction( tr("Move"), this ) )
, mpMenuMoveDirectory( new QAction( tr("Move Directory"), this ) )
, mpMenuMoveContent( new QAction( tr("Move Content"), this ) )
, mpMenuRename( new QAction( tr("Rename"), this ) )
, mpMenuDelete( new QAction( tr("Delete"), this ) )
, mContextModelIndex()
, mFileInfo()
{
   QCompleter *completer = new QCompleter( this );
   completer->setModel( new QDirModel( QStringList(),
                                       QDir::NoDotAndDotDot | QDir::AllDirs,
                                       QDir::Name,
                                       completer ) );
   mpRootDir->setCompleter( completer );

   mpModel->setNameFilters( Settings::value( Settings::RubberbandmanFileExtensions ) );
   mpModel->setFilter( QDir::NoDotAndDotDot | QDir::AllDirs | QDir::Files );
   mpModel->setSorting( QDir::Name | QDir::DirsFirst | QDir::IgnoreCase /*| QDir::LocaleAware*/ );
   mpModel->setLazyChildCount( true );

   mpView->setContextMenuPolicy( Qt::CustomContextMenu );
   mpView->setModel( mpModel );
   mpView->setDragDropMode( QAbstractItemView::DragOnly );

   QVBoxLayout *layout = new QVBoxLayout( this );
   QHBoxLayout *topLayout = new QHBoxLayout;

   topLayout->addWidget( new QLabel(tr("Root:")) );
   topLayout->addWidget( mpRootDir );
   topLayout->addWidget( mpDotButton );
   topLayout->setContentsMargins( 0, 0, 0, 0 );

   layout->addLayout( topLayout );
   layout->addWidget( mpView );
   layout->setContentsMargins( 0, 0, 0, 0 );
   setLayout(layout);

   /* evil hack */
   mpDotButton->setMaximumWidth( mpDotButton->height() );

   mpRootDir->setText( Settings::value( Settings::RubberbandmanRootDirectory ) );
   handleRootDir();
   mpView->setAnimated( true );
   connect( mpView, SIGNAL(clicked(QModelIndex)),
            this, SLOT(entryClicked(QModelIndex)) );
   connect( mpRootDir, SIGNAL(returnPressed()),
            this, SLOT(handleRootDir()) );
   connect( mpDotButton, SIGNAL(clicked()),
            this, SLOT(handleDotButton()) );
   connect( mpView, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(contextMenu(QPoint)) );
   connect( mpMenuSendToPartyman, SIGNAL(triggered()),
            this, SLOT(menuSendToPartyman()) );
   connect( mpMenuRescan, SIGNAL(triggered()),
            this, SLOT(handleRootDir()) );
   connect( mpMenuMoveFile, SIGNAL(triggered()),
            this, SLOT(menuMove()) );
   connect( mpMenuMoveDirectory, SIGNAL(triggered()),
            this, SLOT(menuMove()) );
   connect( mpMenuMoveContent, SIGNAL(triggered()),
            this, SLOT(menuMoveContent()) );
   connect( mpMenuRename, SIGNAL(triggered()),
            this, SLOT(menuRename()) );
   connect( mpMenuDelete, SIGNAL(triggered()),
            this, SLOT(menuDelete()) );

   setAcceptDrops( true );
}


FileSysBrowser::~FileSysBrowser()
{
}


void FileSysBrowser::entryClicked( const QModelIndex &index )
{
   emit clicked( mpModel->filePath(index) );
}


void FileSysBrowser::handleDotButton()
{
   QFileDialog fileDialog( this );

   fileDialog.setFileMode( QFileDialog::DirectoryOnly );
   fileDialog.setDirectory( mpRootDir->text() );
   fileDialog.setReadOnly( true );

   if( fileDialog.exec() )
   {
      QString result( fileDialog.selectedFiles().at(0) );
      mpRootDir->setText( result );
      handleRootDir();
   }
}


void FileSysBrowser::handleRootDir()
{
   QModelIndex qmi( mpModel->index( mpRootDir->text() ) );
   if( qmi.isValid() )
   {
      QString current( mpModel->filePath( mpView->currentIndex() ) );
      Settings::setValue( Settings::RubberbandmanRootDirectory, mpRootDir->text() );
      mpModel->refresh( qmi );
      mpView->setRootIndex( qmi );
      mpView->setCurrentIndex( mpModel->index( current ) );
      scrollTo( current );
   }
   mpView->header()->hide();
   mpView->setColumnHidden( 1, true );
   mpView->setColumnHidden( 2, true );
   mpView->setColumnHidden( 3, true );
}


void FileSysBrowser::scrollTo( const QString &fileName )
{
   QModelIndex qmi( mpModel->index( fileName ) );
   mpView->scrollTo( qmi, QAbstractItemView::PositionAtCenter );
   mpView->setCurrentIndex( qmi );
   // \todo: only emit if in visible area
   emit clicked( fileName );
}


void FileSysBrowser::contextMenu( const QPoint &pos )
{
   mContextModelIndex = mpView->indexAt( pos );
   if( !mContextModelIndex.isValid() )
   {
      return;
   }
   mFileInfo.setFile( mpModel->filePath( mContextModelIndex ) );

   QMenu menu(mpView);
   menu.addAction( mpMenuRescan );
   if( !mFileInfo.isDir() )
   {
      menu.addAction( mpMenuSendToPartyman );
   }
   menu.addSeparator();
   if( mFileInfo.isDir() )
   {
      menu.addAction( mpMenuMoveDirectory );
      menu.addAction( mpMenuMoveContent );
   }
   else
   {
      menu.addAction( mpMenuMoveFile );
   }
   menu.addAction( mpMenuRename );
   menu.addAction( mpMenuDelete );
   menu.exec( mpView->mapToGlobal( pos ) );
}



void FileSysBrowser::menuSendToPartyman()
{
   if( !mFileInfo.isDir() )
   {
      QString msg( mFileInfo.filePath() );
      msg.prepend( "P0Q\n" );
      if( Satellite::get() )
      {
         Satellite::get()->send( msg.toUtf8() );
      }
   }
}


void FileSysBrowser::menuMoveContent()
{
   menuMove( true );
}


void FileSysBrowser::menuMove( bool contentOnly )
{
//TRACESTART(FileSysBrowser::menuMove)
   QString dialogMessage( QApplication::applicationName() + ": " );
   if( contentOnly )
   {
      dialogMessage.append( tr("Move Content Of %1 To:") );
   }
   else
   {
      dialogMessage.append( tr("Move %1 To:") );
   }
   QFileDialog dialog( this, dialogMessage.arg( mFileInfo.fileName()) );
   dialog.setFileMode( QFileDialog::DirectoryOnly );
   dialog.setDirectory( mpRootDir->text() );
   dialog.setAcceptMode( QFileDialog::AcceptOpen );
   if( dialog.exec() )
   {
      QString dest( dialog.selectedFiles().at(0) );

      if( !contentOnly )
      {
         dest.append( QDir::separator() );
         dest.append( mFileInfo.fileName() );
      }
//TRACEMSG << contentOnly << mFileInfo.absoluteFilePath() << dest;
      if( mFileInfo.isDir() )
      {
         QDir qDir;
         DirWalkerMove walkerCallbacks( mFileInfo.absoluteFilePath(), dest );
         DirWalker     dirWalker;
//TRACEMSG << "create" << dest;
         qDir.mkdir( dest );
         dirWalker.run( &walkerCallbacks, mFileInfo.absoluteFilePath() );
//TRACEMSG << "remove" << mFileInfo.absoluteFilePath();
         qDir.rmdir( mFileInfo.absoluteFilePath() );
      }
      else
      {
         QFile::rename( mFileInfo.absoluteFilePath(), dest );
      }

      if( Settings::value( Settings::RubberbandmanAutoRescan ) )
      {
         handleRootDir();
      }
   }
}


void FileSysBrowser::menuRename()
{
   bool ok;
   QString text = QInputDialog::getText( this, QString(QApplication::applicationName() + ": " +
                                                       tr("Rename %1 To:")).arg(mFileInfo.fileName()),
                                         tr("Rename %1 To:").arg(mFileInfo.fileName()),
                                         QLineEdit::Normal, mFileInfo.fileName(), &ok );
   if (ok && !text.isEmpty())
   {
      QDir qd( mFileInfo.absolutePath() );
      QFileInfo qfi( mFileInfo.absolutePath() + "/" + text );

      if( qfi.absolutePath() == mFileInfo.absolutePath() )
      {
         if( qd.rename( mFileInfo.fileName(), text ) )
         {
            mpDatabase->rename( mFileInfo.absoluteFilePath(), qfi.absoluteFilePath() );
            if( Settings::value( Settings::RubberbandmanAutoRescan ) )
            {
               handleRootDir();
            }
         }
      }
      else
      {
         QMessageBox::warning( this, QApplication::applicationName(),
                               tr("New name is not valid.") );
      }
   }
}


void FileSysBrowser::menuDelete()
{
   QMessageBox::StandardButton button;
   button = QMessageBox::question( this, QString(QApplication::applicationName() + ": " + tr("Delete %1")).arg(mFileInfo.fileName()),
                                   tr("Really Delete %1 ?").arg(mFileInfo.fileName()),
                                   QMessageBox::Ok | QMessageBox::Cancel );
   if( button == QMessageBox::Ok )
   {
      QDir qdir( mFileInfo.absolutePath() );
      if( mFileInfo.isDir() )
      {
         DirWalkerDelete walkerCallbacks;
         DirWalker       dirWalker;
         dirWalker.run( &walkerCallbacks, mFileInfo.absoluteFilePath() );
         qdir.rmdir( mFileInfo.fileName() );
      }
      else
      {
         qdir.remove( mFileInfo.fileName() );
      }
      if( Settings::value( Settings::RubberbandmanAutoRescan ) )
      {
         mpView->setCurrentIndex( mpView->indexAbove( mContextModelIndex ) );
         handleRootDir();
      }
   }
}


void FileSysBrowser::dragEnterEvent( QDragEnterEvent *event )
{
//TRACESTART(FileSysBrowser::dragEnterEvents)
   const QMimeData *mimeData = event->mimeData();
   if( mimeData->hasUrls() && (mimeData->urls().size() == 1) )
   {
      if( !mimeData->urls().at(0).toLocalFile().isEmpty() )
      {
         event->acceptProposedAction();
         return;
      }
   }
   event->ignore();
}


void FileSysBrowser::dropEvent( QDropEvent *event )
{
   const QMimeData *mimeData = event->mimeData();
   if( mimeData->hasUrls() && (mimeData->urls().size() == 1) )
   {
      if( !mimeData->urls().at(0).toLocalFile().isEmpty() )
      {
         event->accept();
         scrollTo( mimeData->urls().at(0).toLocalFile() );
         return;
      }
   }
   event->ignore();
}
