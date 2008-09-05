/**
 * FileSysBrowser.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#include "FileSysBrowser.hpp"
#include "MySettings.hpp"
#include "Database.hpp"
#include "DirWalker.hpp"

#include <QtGui>
#include <QString>

#include "Trace.hpp"


class DirWalkerDelete : public DirWalkerCallbacks
{
public:
   DirWalkerDelete( Database *database, const QString &path )
   : mpDatabase( database )
   , mDir( path )
   , mDirChanged( true )
   , mTrackInfo()
   {
   }
   virtual ~DirWalkerDelete(){}
   void handleFile( const QFileInfo &fileInfo )
   {
      mDir.setPath( fileInfo.absolutePath() );
      mDir.remove( fileInfo.fileName() );
      if( mpDatabase->getTrackInfo( &mTrackInfo, fileInfo.absoluteFilePath() ) )
      {
         mpDatabase->deleteTrackInfo( &mTrackInfo );
      }
   }
   void handleDir( const QFileInfo &fileInfo )
   {
      mDir.setPath( fileInfo.absolutePath() );
      mDir.rmdir( fileInfo.fileName() );
   }
   void handleOther( const QFileInfo &fileInfo )
   {
      mDir.setPath( fileInfo.absolutePath() );
      mDir.remove( fileInfo.fileName() );
   }
   
private:
   Database  *mpDatabase;
   QDir      mDir;
   bool      mDirChanged;
   TrackInfo mTrackInfo;
};



class MyTreeView : public QTreeView
{
public:
   MyTreeView( QWidget *parent ) : QTreeView( parent ){};
protected:
   /* handle return/enter key */
   virtual void keyPressEvent( QKeyEvent *event );
};


void MyTreeView::keyPressEvent( QKeyEvent *event )
{
   if( !event->isAutoRepeat() )
   {
      switch( event->key() )
      {
         case Qt::Key_Return:
         case Qt::Key_Enter:
            emit clicked( currentIndex() );
            break;
         default:
            break;
      }
   }
   QTreeView::keyPressEvent( event );
}


FileSysBrowser::FileSysBrowser( Database *database, QWidget *parent, Qt::WindowFlags flags )
: QWidget( parent, flags )
, mpDatabase( database )
, mpRootDir( new QLineEdit( this ) )
, mpDotButton( new QPushButton( tr("..."), this ) )
, mpView( new MyTreeView( this ) )
, mpModel( new QDirModel( this ) )
, mpMenuSendToPartyman( new QAction( tr("Send To Partyman"), this ) )
, mpMenuRescan( new QAction( tr("Rescan"), this ) )
, mpMenuMove( new QAction( tr("Move"), this ) )
, mpMenuRename( new QAction( tr("Rename"), this ) )
, mpMenuDelete( new QAction( tr("Delete"), this ) )
, mContextModelIndex()
, mFileInfo()
{
   MySettings settings;
   
   QStringList defaultNameFilters;
   defaultNameFilters << "*.mp3" << "*.ogg";
   mpModel->setNameFilters( settings.value( "FileExtensions", defaultNameFilters ).toStringList() );
   mpModel->setFilter( QDir::NoDotAndDotDot | QDir::AllDirs | QDir::Files );
   mpModel->setSorting( QDir::Name | QDir::DirsFirst | QDir::IgnoreCase /*| QDir::LocaleAware*/ );
   mpModel->setLazyChildCount( true );
   
   mpView->setContextMenuPolicy( Qt::CustomContextMenu );
   mpView->setModel( mpModel );
   
   QVBoxLayout *layout = new QVBoxLayout( this );
   QHBoxLayout *topLayout = new QHBoxLayout;
   
   topLayout->addWidget( new QLabel(tr("Root:")) );
   topLayout->addWidget( mpRootDir );
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
   
   /* evil hack */
   mpDotButton->setMaximumWidth( mpDotButton->height() );
   
   mpRootDir->setText( settings.value( "RootDirectory", QString("/") ).toString() );
   handleRootDir();
   mpView->setAnimated( true );
   connect( mpView, SIGNAL(clicked(const QModelIndex&)),
            this, SLOT(entryClicked(const QModelIndex&)) );
   connect( mpRootDir, SIGNAL(returnPressed()),
            this, SLOT(handleRootDir()) );
   connect( mpDotButton, SIGNAL(clicked()),
            this, SLOT(handleDotButton()) );
   connect( mpView, SIGNAL(customContextMenuRequested(const QPoint&)),
            this, SLOT(contextMenu(const QPoint&)) );
   connect( mpMenuSendToPartyman, SIGNAL(triggered()),
            this, SLOT(menuSendToPartyman()) );
   connect( mpMenuRescan, SIGNAL(triggered()),
            this, SLOT(handleRootDir()) );
   connect( mpMenuMove, SIGNAL(triggered()),
            this, SLOT(menuMove()) );
   connect( mpMenuRename, SIGNAL(triggered()),
            this, SLOT(menuRename()) );
   connect( mpMenuDelete, SIGNAL(triggered()),
            this, SLOT(menuDelete()) );
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
      MySettings().setValue( "RootDirectory", mpRootDir->text() );
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
   /* TODO: only emit if in visible area */
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
   menu.addAction( mpMenuMove );
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
      MySettings().sendUdpMessage( msg, "Partyman" );
   }
}


void FileSysBrowser::menuMove()
{
TRACESTART(FileSysBrowser::menuMove)
   QFileDialog dialog( this, QString(tr("Rubberbandman: Move %1 To:")).arg(mFileInfo.fileName()) );
   dialog.setFileMode( QFileDialog::DirectoryOnly );
   dialog.setDirectory( mpRootDir->text() );
   dialog.setAcceptMode( QFileDialog::AcceptOpen );
   if( dialog.exec() )
   {
TRACEMSG << dialog.selectedFiles();
      if( MySettings().value("AutoRescan", true).toBool() )
      {
         handleRootDir();
      }
   }
}


void FileSysBrowser::menuRename()
{
   bool ok;
   QString text = QInputDialog::getText( this, QString(tr("Rubberbandman: Rename %1 To:")).arg(mFileInfo.fileName()),
                                         QString(tr("Rename %1 To:")).arg(mFileInfo.fileName()),
                                         QLineEdit::Normal, mFileInfo.fileName(), &ok );
   if (ok && !text.isEmpty())
   {
      QDir qd( mFileInfo.absolutePath() );
      QFileInfo qfi( mFileInfo.absolutePath() + "/" + text );
      
      if( qfi.absolutePath() == mFileInfo.absolutePath() )
      {
         if( qd.rename( mFileInfo.fileName(), text ) )
         {
            if( mFileInfo.isDir() )
            {
               mpDatabase->rename( qfi.absoluteFilePath(), mFileInfo.absoluteFilePath() );
            }
            else
            {
               mpDatabase->rename( qfi.fileName(), mFileInfo.absolutePath(), mFileInfo.fileName() );
            }
            if( MySettings().value("AutoRescan", true).toBool() )
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
   button = QMessageBox::question( this, QString(tr("Rubberbandman: Delete %1")).arg(mFileInfo.fileName()),
                                   QString(tr("Really Delete %1 ?")).arg(mFileInfo.fileName()),
                                   QMessageBox::Ok | QMessageBox::Cancel );
   if( button == QMessageBox::Ok )
   {
      QDir qdir( mFileInfo.absolutePath() );
      if( mFileInfo.isDir() )
      {
         DirWalkerDelete walkerCallbacks( mpDatabase, mFileInfo.absoluteFilePath() );
         DirWalker       dirWalker;
         dirWalker.run( &walkerCallbacks, mFileInfo.absoluteFilePath(), DirWalker::RecurseBeforeCallback );
         qdir.rmdir( mFileInfo.fileName() );
      }
      else
      {
         qdir.remove( mFileInfo.fileName() );
      }
      if( MySettings().value("AutoRescan", true).toBool() )
      {
         handleRootDir();
      }
   }
}
