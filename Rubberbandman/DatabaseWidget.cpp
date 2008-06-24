/**
 * DatabaseWidget.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#include "DatabaseWidget.hpp"

#include <QtGui>
#include <QString>

#include <fileref.h>
#include <tag.h>

#include "FileSysBrowser.hpp"
#include "InfoEdit.hpp"
#include "DirWalker.hpp"
#include "MySettings.hpp"

#include "Trace.hpp"

DatabaseWidget::DatabaseWidget( QWidget *parent, Qt::WindowFlags flags )
: QWidget( parent, flags )
, mpDatabase( new Database() )
, mpQueryModel( new QSqlQueryModel() )
, mpTableView( new QTableView() )
{
   QPushButton *updateButton = new QPushButton( tr("Update") );
   QPushButton *cleanupButton = new QPushButton( tr("Clean up") );
   
   connect( updateButton, SIGNAL(pressed()), this, SLOT(handleUpdate()) );
   connect( cleanupButton, SIGNAL(pressed()), this, SLOT(handleCleanup()) );
   
   mpQueryModel->setQuery( "SELECT id,Directory,FileName,Artist,Title,Album,TrackNr,Year,Genre,"
                           "PlayTime,LastModified,TimesPlayed,Volume,Folders,Flags FROM slart_tracks;" );
   mpTableView->setModel( mpQueryModel );
   
   QVBoxLayout *layout = new QVBoxLayout;
   QHBoxLayout *buttonLayout = new QHBoxLayout;
   buttonLayout->addWidget( updateButton );
   buttonLayout->addWidget( cleanupButton );
   layout->addWidget( mpTableView );
   layout->addLayout( buttonLayout );
   setLayout(layout);
}


void DatabaseWidget::handleUpdate()
{
   TrackInfoList trackInfoList;
   DirWalker walker;
   connect( &walker, SIGNAL(foundFile(const QFileInfo&)),
            this, SLOT(handleFile(const QFileInfo&)) );
   mpDatabase->beginTransaction();
   walker.run( MySettings().value( "RootDirectory", QString("/") ).toString(), true );
   mpDatabase->endTransaction(true);
   disconnect( &walker, SIGNAL(foundFile(const QFileInfo&)),
               this, SLOT(handleFile(const QFileInfo&)) );
   QString query( mpQueryModel->query().lastQuery() );
   delete mpQueryModel;
   mpQueryModel = new QSqlQueryModel();
   mpQueryModel->setQuery( query );
   mpTableView->setModel( mpQueryModel );
}


void DatabaseWidget::handleCleanup()
{
   TrackInfoList   trackInfoList;
   const TrackInfo *trackInfo;
   int s = mpDatabase->getTrackInfoList( &trackInfoList );
   
   QFileInfo qfi;
   for( int i = 0; i < s; i++ )
   {
      trackInfo = &(trackInfoList.at(i));
      qfi.setFile( trackInfo->mDirectory + "/" + trackInfo->mFileName );
      if( !qfi.isFile() )
      {
         mpDatabase->deleteTrackInfo( trackInfo );
      }
   }
}


void DatabaseWidget::handleFile( const QFileInfo &fileInfo )
{
   TrackInfo *trackInfo = new TrackInfo();
   
   if( !mpDatabase->getTrackInfoByFileName( trackInfo, fileInfo.absoluteFilePath() ) )
   {
      QString fileName(fileInfo.absoluteFilePath());
      int fileNameStart = fileName.lastIndexOf('/');
      
      trackInfo->mID           = 0;
      trackInfo->mDirectory    = fileName.left(fileNameStart);
      trackInfo->mFileName     = fileName.mid(fileNameStart+1);
      trackInfo->mLastModified = 0;
      trackInfo->mTimesPlayed  = 0;
      trackInfo->mFlags        = 0;
   }
   
   if( updateTrackInfoFromFile( trackInfo, fileInfo.absoluteFilePath() ) )
   {
      mpDatabase->updateTrackInfo( trackInfo );
   }
   
   delete trackInfo;
}


bool DatabaseWidget::updateTrackInfoFromFile( TrackInfo *trackInfo, const QString &fileName )
{   
   QFileInfo fileInfo( fileName );
   
   if( fileInfo.lastModified().toTime_t() > trackInfo->mLastModified )
   {
      TagLib::FileRef f( fileName.toLocal8Bit().data() );
      if( f.file() )
      {
         trackInfo->mArtist       = QString::fromUtf8( f.tag()->artist().toCString( true ) );
         trackInfo->mTitle        = QString::fromUtf8( f.tag()->title().toCString( true ) );
         trackInfo->mAlbum        = QString::fromUtf8( f.tag()->album().toCString( true ) );
         trackInfo->mTrackNr      = f.tag()->track();
         trackInfo->mYear         = f.tag()->year();
         trackInfo->mGenre        = QString::fromUtf8( f.tag()->genre().toCString( true ) );
         trackInfo->mPlayTime     = f.audioProperties()->length();
         trackInfo->mLastModified = fileInfo.lastModified().toTime_t();
         
         return true;
      }
   }
   
   return false;
}

