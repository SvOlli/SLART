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

#include "Trace.hpp"

DatabaseWidget::DatabaseWidget( QWidget *parent, Qt::WindowFlags flags )
: QWidget( parent, flags )
, mpDatabase( new Database() )
{
   QPushButton *updateButton = new QPushButton( tr("Update") );
   QPushButton *cleanupButton = new QPushButton( tr("Clean up") );
   
   connect( updateButton, SIGNAL(pressed()), this, SLOT(handleUpdate()) );
   connect( cleanupButton, SIGNAL(pressed()), this, SLOT(handleCleanup()) );
   
   QVBoxLayout *layout = new QVBoxLayout;
   layout->addWidget( updateButton );
   layout->addWidget( cleanupButton );
   setLayout(layout);
}


void DatabaseWidget::handleUpdate()
{
   TrackInfoList trackInfoList;
   DirWalker walker;
   connect( &walker, SIGNAL(foundFile(const QFileInfo&)),
            this, SLOT(handleFile(const QFileInfo&)) );
   mpDatabase->beginTransaction();
   walker.run( "/media/share/Media/Music", true );
   mpDatabase->endTransaction(true);
   disconnect( &walker, SIGNAL(foundFile(const QFileInfo&)),
               this, SLOT(handleFile(const QFileInfo&)) );
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

