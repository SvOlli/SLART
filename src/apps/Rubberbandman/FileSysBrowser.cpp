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
#include <QAction>
#include <QApplication>
#include <QCompleter>
#include <QDirModel>
#include <QDragEnterEvent>
#include <QFileDialog>
#include <QFileSystemModel>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QInputDialog>
#include <QLabel>
#include <QLineEdit>
#include <QMenu>
#include <QMessageBox>
#include <QMimeData>
#include <QPushButton>
#include <QString>
#include <QTimer>
#include <QUrl>
#include <QVBoxLayout>

/* local library headers */
#include <DatabaseInterface.hpp>
#include <DirWalker.hpp>
#include <Settings.hpp>
#include <Satellite.hpp>

/* local headers */
#include "DirWalkerDelete.hpp"
#include "DirWalkerMove.hpp"
#include "FileIconProvider.hpp"
#include "MyTreeView.hpp"


#include <QtDebug>

class MyFileSystemModel : public QFileSystemModel
{
public:
   MyFileSystemModel( QObject *parent )
   :QFileSystemModel( parent )
   {
   }

   virtual bool	canFetchMore ( const QModelIndex & parent ) const
   {
      qDebug() << "canFetchMore";
      return QFileSystemModel::canFetchMore( parent );
   }
   virtual int	columnCount ( const QModelIndex & parent = QModelIndex() ) const
   {
      int retval = QFileSystemModel::columnCount( parent );
      qDebug() << "columnCount" << retval;
      return retval;
   }
   virtual QVariant	data ( const QModelIndex & index, int role = Qt::DisplayRole ) const
   {
      QVariant retval( QFileSystemModel::data( index, role ) );
      qDebug() << "data" << retval;
      return retval;
   }
   virtual bool	dropMimeData ( const QMimeData * data, Qt::DropAction action, int row, int column, const QModelIndex & parent )
   {
      qDebug() << "dropMimeData";
      return QFileSystemModel::dropMimeData( data, action, row, column, parent );
   }
   virtual void	fetchMore ( const QModelIndex & parent )
   {
      qDebug() << "fetchMore";
      return QFileSystemModel::fetchMore( parent );
   }
   virtual Qt::ItemFlags	flags ( const QModelIndex & index ) const
   {
      qDebug() << "flags";
      return QFileSystemModel::flags( index );
   }
   virtual bool	hasChildren ( const QModelIndex & parent = QModelIndex() ) const
   {
      qDebug() << "hasChildren";
      return QFileSystemModel::hasChildren( parent );
   }
   virtual QVariant	headerData ( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const
   {
      qDebug() << "headerData";
      return QFileSystemModel::headerData( section, orientation, role );
   }
   virtual QModelIndex	index ( int row, int column, const QModelIndex & parent = QModelIndex() ) const
   {
      QModelIndex idx( QFileSystemModel::index( row, column, parent ) );
      qDebug() << "index" << idx.data().toString();
      return idx;
   }
   virtual QMimeData *	mimeData ( const QModelIndexList & indexes ) const
   {
      qDebug() << "mimeData";
      return QFileSystemModel::mimeData( indexes );
   }
   virtual QStringList	mimeTypes () const
   {
      qDebug() << "mimeTypes";
      return QFileSystemModel::mimeTypes();
   }
   virtual QModelIndex	parent ( const QModelIndex & index ) const
   {
      qDebug() << "parent";
      return QFileSystemModel::parent( index );
   }
   virtual int	rowCount ( const QModelIndex & parent = QModelIndex() ) const
   {
      int retval = QFileSystemModel::rowCount( parent );
      qDebug() << "rowCount" << retval;
      return retval;
   }
   virtual bool	setData ( const QModelIndex & idx, const QVariant & value, int role = Qt::EditRole )
   {
      qDebug() << "setData";
      return QFileSystemModel::setData( idx, value, role );
   }
   virtual void	sort ( int column, Qt::SortOrder order = Qt::AscendingOrder )
   {
      qDebug() << "sort";
      return QFileSystemModel::sort( column, order );
   }
};


FileSysBrowser::FileSysBrowser( QWidget *parent )
: QWidget( parent )
, mpDatabase( DatabaseInterface::get() )
, mpRootDir( new QLineEdit( this ) )
, mpDotButton( new QPushButton( "...", this ) )
, mpView( new MyTreeView( this ) )
, mpModel( new QFileSystemModel( this ) )
, mpMenuSendToPartyman( new QAction( tr("Send To Partyman"), this ) )
, mpMenuMoveFile( new QAction( tr("Move"), this ) )
, mpMenuMoveDirectory( new QAction( tr("Move Directory"), this ) )
, mpMenuMoveContent( new QAction( tr("Move Content"), this ) )
, mpMenuRename( new QAction( tr("Rename"), this ) )
, mpMenuDelete( new QAction( tr("Delete"), this ) )
, mpFileIconProvider( new FileIconProvider() )
, mContextModelIndex()
, mFileInfo()
{
   const QString rootDir( Settings::value( Settings::RubberbandmanRootDirectory ) );
   QCompleter *completer = new QCompleter( this );

#if 1
   // \todo: migrate QDirModel to QFileSystemModel
   completer->setModel( new QDirModel( QStringList(),
                                       QDir::NoDotAndDotDot | QDir::AllDirs,
                                       QDir::Name,
                                       completer ) );
#else
   // this sucks! QFileSystemModel seems not be up to the job
   QFileSystemModel *dirsModel = new QFileSystemModel( completer );
   dirsModel->setFilter( QDir::NoDotAndDotDot | QDir::AllDirs );
   for( int i = 1; i >= 0; i = rootDir.indexOf( "/", i + 1 ) )
   {
      dirsModel->setRootPath( rootDir.left( i ) );
   }
   dirsModel->setRootPath( rootDir + "/" );
   dirsModel->setRootPath( "/" );
   completer->setModel( dirsModel );
#endif

   mpModel->setRootPath( "/" );
   mpModel->setNameFilters( Settings::value( Settings::RubberbandmanFileExtensions ) );
   mpModel->setFilter( QDir::NoDotAndDotDot | QDir::AllDirs | QDir::Files );
   mpModel->setIconProvider( mpFileIconProvider );

   mpView->setContextMenuPolicy( Qt::CustomContextMenu );
   mpView->setModel( mpModel );
   mpView->setDragDropMode( QAbstractItemView::DragOnly );

   mpRootDir->setCompleter( completer );
   mpRootDir->setText( rootDir );

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
   delete mpFileIconProvider;
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
      const QString current( mpModel->filePath( mpView->currentIndex() ) );
      Settings::setValue( Settings::RubberbandmanRootDirectory, mpRootDir->text() );
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
   QModelIndex qmi;
   for( int i = 1; i >= 0; i = fileName.indexOf( "/", i + 1 ) )
   {
      qmi = mpModel->index( fileName.left( i ) );
      mpView->expand( qmi );
   }
   qmi = mpModel->index( fileName );
   mpView->setCurrentIndex( qmi );
   QTimer::singleShot( 333, mpView, SLOT(scrollToActive()) );
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
   if( !mFileInfo.isDir() )
   {
      menu.addSeparator();
      menu.addAction( mpMenuSendToPartyman );
   }
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
