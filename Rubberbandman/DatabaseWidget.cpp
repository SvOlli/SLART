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
TRACESTART(DatabaseWidget::DatabaseWidget)
   QPushButton *testButton = new QPushButton( tr("Test") );
   
   connect( testButton, SIGNAL(pressed()), this, SLOT(runTest()) );
   
   QVBoxLayout *layout = new QVBoxLayout;
   layout->addWidget( testButton );
   setLayout(layout);
}

void DatabaseWidget::runTest()
{
   DirWalker walker;
   connect( &walker, SIGNAL(foundFile(const QFileInfo&)),
            this, SLOT(handleFile(const QFileInfo&)) );
   mpDatabase->beginTransaction();
   walker.run( "/media/share/Media/Music/!BPM", true );
   mpDatabase->endTransaction(true);
   disconnect( &walker, SIGNAL(foundFile(const QFileInfo&)),
               this, SLOT(handleFile(const QFileInfo&)) );
}

void DatabaseWidget::handleFile( const QFileInfo &fileInfo )
{
TRACESTART(DatabaseWidget::handleFile)
   
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
      mpDatabase->updateTrackInfoByFileName( trackInfo );
   }
   
   delete trackInfo;
}


bool DatabaseWidget::updateTrackInfoFromFile( TrackInfo *trackInfo, const QString &fileName )
{   
TRACESTART(DatabaseWidget::updateTrackInfoFromFile)
   QFileInfo fileInfo( fileName );
   
   if( fileInfo.lastModified().toTime_t() > trackInfo->mLastModified )
   {
TRACEMSG << fileInfo.lastModified().toTime_t() << ">" << trackInfo->mLastModified ;
   
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

