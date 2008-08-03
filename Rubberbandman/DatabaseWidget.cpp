/**
 * DatabaseWidget.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#include "DatabaseWidget.hpp"

#include <QtGui>
#include <QString>
#include <QSqlTableModel>
#include <QTableView>

#include <fileref.h>
#include <tag.h>

#include "Database.hpp"
//#include "FileSysBrowser.hpp"
//#include "InfoEdit.hpp"
#include "DirWalker.hpp"
#include "MySettings.hpp"

#include "Trace.hpp"

DatabaseWidget::DatabaseWidget( Database *database, QWidget *parent, Qt::WindowFlags flags )
: QWidget( parent, flags )
, mpDatabase( database )
, mpBaseDir( new QLineEdit( this ) )
, mpUpdateButton( new QPushButton( tr("Update"), this ) )
, mpMessage( new QLabel( this ) )
#if 0
, mpTableModel( new QSqlTableModel() )
, mpTableView( new QTableView() )
#endif
, mTrackInfo()
{
   QPushButton *cleanupButton = new QPushButton( tr("Clean up"), this );
   QPushButton *browseButton  = new QPushButton( tr("Browse"), this );
   
   connect( browseButton, SIGNAL(pressed()),
            this, SLOT(setBaseDir()) );
   connect( mpUpdateButton, SIGNAL(pressed()),
            this, SLOT(handleUpdate()) );
   connect( cleanupButton, SIGNAL(pressed()),
            this, SLOT(handleCleanup()) );
   connect( mpBaseDir, SIGNAL(textChanged(const QString &)),
            this, SLOT(checkValidDir(const QString &)) );
   
#if 0
   mpTableModel->setQuery( "SELECT id,Directory,FileName,Artist,Title,Album,TrackNr,Year,Genre,"
                           "PlayTime,LastModified,TimesPlayed,Volume,Folders,Flags FROM slart_tracks;" );
   mpTableModel->setTable( "slart_tracks" );
   mpTableView->setModel( mpTableModel );
   mpTableModel->select();
#endif
   
   QVBoxLayout *layout = new QVBoxLayout;
   QHBoxLayout *rootLayout = new QHBoxLayout;
   rootLayout->addWidget( new QLabel( tr("Music Base:"), this ) );
   rootLayout->addWidget( mpBaseDir );
   rootLayout->addWidget( browseButton );
   
   QHBoxLayout *buttonLayout = new QHBoxLayout;
   buttonLayout->addWidget( mpUpdateButton );
   buttonLayout->addWidget( cleanupButton );
#if 0
   layout->addWidget( mpTableView );
#endif
   layout->addLayout( rootLayout );
   layout->addLayout( buttonLayout );
   layout->addWidget( mpMessage );
   layout->addStretch();
   setLayout(layout);
   mpBaseDir->setText( MySettings( "Global" ).value( "MusicBase", QString("/") ).toString() );
}


void DatabaseWidget::handleUpdate()
{
   TrackInfoList trackInfoList;
   DirWalker walker;
   mCount = 0;
   mLastCount = 0;
   connect( &walker, SIGNAL(foundFile(const QFileInfo&)),
            this, SLOT(handleFile(const QFileInfo&)) );
   connect( &walker, SIGNAL(foundDir(const QFileInfo&)),
            this, SLOT(handleDir(const QFileInfo&)) );
   mpDatabase->beginTransaction();
   walker.run( mpBaseDir->text(), true );
   mpDatabase->endTransaction(true);
   disconnect( &walker, SIGNAL(foundFile(const QFileInfo&)),
               this, SLOT(handleFile(const QFileInfo&)) );
   disconnect( &walker, SIGNAL(foundDir(const QFileInfo&)),
               this, SLOT(handleDir(const QFileInfo&)) );
   mpMessage->setText( "Done, " + QString::number(mCount) + " files scanned." );
   if( mCount > 3 )
   {
      emit databaseOk();
   }
#if 0
   mpTableModel->select();
   QString query( mpTableModel->query().lastQuery() );
   delete mpTableModel;
   mpTableModel = new QSqlTableModel();
   mpTableModel->setQuery( query );
   mpTableView->setModel( mpTableModel );
#endif
}


void DatabaseWidget::handleCleanup()
{
   TrackInfoList   trackInfoList;
   int s = mpDatabase->getTrackInfoList( &trackInfoList );
   
   QFileInfo qfi;
   for( int i = 0; i < s; i++ )
   {
      mTrackInfo = trackInfoList.at(i);
      qfi.setFile( mTrackInfo.mDirectory + "/" + mTrackInfo.mFileName );
      if( !qfi.isFile() )
      {
         mpDatabase->deleteTrackInfo( &mTrackInfo );
      }
   }
}


void DatabaseWidget::handleDir( const QFileInfo &/*fileInfo*/ )
{
   if( mCount > mLastCount + 250 )
   {
      mpMessage->setText( QString::number(mCount) + " files scanned." );
      QCoreApplication::processEvents();
      mLastCount = mCount;
   }
}


void DatabaseWidget::handleFile( const QFileInfo &fileInfo )
{
   ++mCount;
   mTrackInfo.mID = 0;
   if( !mpDatabase->getTrackInfo( &mTrackInfo, fileInfo.absoluteFilePath() ) )
   {
      QString fileName( fileInfo.absoluteFilePath() );
      int fileNameStart = fileName.lastIndexOf('/');
      
      mTrackInfo.mID           = 0;
      mTrackInfo.mDirectory    = fileName.left(fileNameStart);
      mTrackInfo.mFileName     = fileName.mid(fileNameStart+1);
      mTrackInfo.mLastTagsRead = 0;
      mTrackInfo.mTimesPlayed  = 0;
      mTrackInfo.mFlags        = 0;
   }
   
   if( updateTrackInfoFromFile( fileInfo.absoluteFilePath() ) )
   {
      mpDatabase->updateTrackInfo( &mTrackInfo );
   }
}


bool DatabaseWidget::updateTrackInfoFromFile( const QString &fileName )
{   
   QFileInfo fileInfo( fileName );
   
   if( fileInfo.lastModified().toTime_t() > mTrackInfo.mLastTagsRead )
   {
      TagLib::FileRef f( fileName.toLocal8Bit().data() );
      if( f.file() && f.tag() )
      {
         mTrackInfo.mArtist       = QString::fromUtf8( f.tag()->artist().toCString( true ) );
         mTrackInfo.mTitle        = QString::fromUtf8( f.tag()->title().toCString( true ) );
         mTrackInfo.mAlbum        = QString::fromUtf8( f.tag()->album().toCString( true ) );
         mTrackInfo.mTrackNr      = f.tag()->track();
         mTrackInfo.mYear         = f.tag()->year();
         mTrackInfo.mGenre        = QString::fromUtf8( f.tag()->genre().toCString( true ) );
         if( f.audioProperties() )
         {
            mTrackInfo.mPlayTime  = f.audioProperties()->length();
         }
         mTrackInfo.mLastTagsRead = fileInfo.lastModified().toTime_t();
         
         return true;
      }
   }
   
   return false;
}


void DatabaseWidget::setBaseDir()
{
   QFileDialog fileDialog( this );
   
   fileDialog.setFileMode( QFileDialog::DirectoryOnly );
   fileDialog.setDirectory( mpBaseDir->text() );
   fileDialog.setReadOnly( true );
   
   if( fileDialog.exec() )
   {
      MySettings settings( "Global" );
      QString result( fileDialog.selectedFiles().at(0) );
      mpBaseDir->setText( result );
      settings.setValue( "MusicBase", result.replace('\\','/') );
   }
}


void DatabaseWidget::checkValidDir( const QString &dirName )
{
   QFileInfo qfi( dirName );
   if( qfi.isDir() )
   {
      mpUpdateButton->setDisabled( false );
      MySettings settings( "Global" );
      settings.setValue( "MusicBase", QString( dirName ).replace('\\','/') );
   }
   else
   {
      mpUpdateButton->setDisabled( true );
   }
}
