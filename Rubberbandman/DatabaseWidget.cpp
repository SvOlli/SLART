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
#include "FileSysBrowser.hpp"
#include "InfoEdit.hpp"
#include "DirWalker.hpp"
#include "MySettings.hpp"

#include "Trace.hpp"

DatabaseWidget::DatabaseWidget( Database *database, QWidget *parent, Qt::WindowFlags flags )
: QWidget( parent, flags )
, mpDatabase( database )
, mpTableModel( new QSqlTableModel() )
, mpTableView( new QTableView() )
{
   QPushButton *updateButton = new QPushButton( tr("Update") );
   QPushButton *cleanupButton = new QPushButton( tr("Clean up") );
   
   connect( updateButton, SIGNAL(pressed()), this, SLOT(handleUpdate()) );
   connect( cleanupButton, SIGNAL(pressed()), this, SLOT(handleCleanup()) );
   
#if 0
   mpTableModel->setQuery( "SELECT id,Directory,FileName,Artist,Title,Album,TrackNr,Year,Genre,"
                           "PlayTime,LastModified,TimesPlayed,Volume,Folders,Flags FROM slart_tracks;" );
   mpTableModel->setTable( "slart_tracks" );
   mpTableView->setModel( mpTableModel );
   mpTableModel->select();
#endif
   
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
   mpTableModel->select();
#if 0
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
   
   if( !mpDatabase->getTrackInfo( trackInfo, fileInfo.absoluteFilePath() ) )
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
      if( f.file() && f.tag() )
      {
         trackInfo->mArtist       = QString::fromUtf8( f.tag()->artist().toCString( true ) );
         trackInfo->mTitle        = QString::fromUtf8( f.tag()->title().toCString( true ) );
         trackInfo->mAlbum        = QString::fromUtf8( f.tag()->album().toCString( true ) );
         trackInfo->mTrackNr      = f.tag()->track();
         trackInfo->mYear         = f.tag()->year();
         trackInfo->mGenre        = QString::fromUtf8( f.tag()->genre().toCString( true ) );
         if( f.audioProperties() )
         {
            trackInfo->mPlayTime  = f.audioProperties()->length();
         }
         trackInfo->mLastModified = fileInfo.lastModified().toTime_t();
         
         return true;
      }
   }
   
   return false;
}

