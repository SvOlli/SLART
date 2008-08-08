/**
 * DatabaseWorker.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#include "DatabaseWorker.hpp"
#include "Database.hpp"

#include <QtGui>
#include <QFileInfo>

#include <fileref.h>
#include <tag.h>

#include "Trace.hpp"


class DirWalkerDatabaseUpdate : public DirWalkerCallbacks
{
public:
   DirWalkerDatabaseUpdate( DatabaseWorker *databaseWorker )
   : mpDatabaseWorker( databaseWorker )
   {
   }
   virtual ~DirWalkerDatabaseUpdate(){}
   void handleFile( const QFileInfo &fileInfo )
   {
      mpDatabaseWorker->updateFile( fileInfo );
   }
   void handleDir( const QFileInfo &fileInfo )
   {
      mpDatabaseWorker->updateDir( fileInfo );
   }
   void handleOther( const QFileInfo &/*fileInfo*/ )
   {
   }
private:
   DatabaseWorker *mpDatabaseWorker;
};


void DatabaseWorker::prepare( Database *database )
{
   mpDatabase  = database;
   mMode       = none;
}


bool DatabaseWorker::initUpdate( const QString &baseDir )
{
   if( mMode != none )
   {
      return false;
   }
   mMode   = update;
   mCancel = false;
   mPath   = baseDir;
   
   return true;
}


bool DatabaseWorker::initCleanup()
{
   if( mMode != none )
   {
      return false;
   }
   mMode   = cleanup;
   mCancel = false;
   
   return true;
}


bool DatabaseWorker::initImport( const QString &fileName )
{
   if( mMode != none )
   {
      return false;
   }
   mMode   = import;
   mCancel = false;
   mPath   = fileName;
   
   return true;
}


void DatabaseWorker::run()
{
   mChecked   = 0;
   mProcessed = 0;
   mpDatabase->beginTransaction();
   switch( mMode )
   {
      case update:
         {
            DirWalkerDatabaseUpdate walkerCallbacks( this );
            mDirWalker.run( &walkerCallbacks, mPath, DirWalker::RecurseBeforeCallback );
         }
         break;
      case cleanup:
         {
            TrackInfoList trackInfoList;
            QFileInfo     qfi;
            int           s = mpDatabase->getTrackInfoList( &trackInfoList );
            for( mChecked = 0; mChecked < s; mChecked++ )
            {
               mTrackInfo = trackInfoList.at( mChecked );
               qfi.setFile( mTrackInfo.mDirectory + "/" + mTrackInfo.mFileName );
               if( !qfi.isFile() )
               {
                  mpDatabase->deleteTrackInfo( &mTrackInfo );
                  ++mProcessed;
               }
               if( mChecked % 200 == (200 - 1) )
               {
                  emit progress( mChecked + 1, mProcessed );
               }
            }
         }
         break;
      case import:
         break;
      default:
         break;
   }
   mpDatabase->endTransaction( true );
   emit progress( mChecked, mProcessed );
   mMode = none;
}


void DatabaseWorker::updateDir( const QFileInfo &/*fileInfo*/ )
{
   if( mChecked > mLastChecked + 200 )
   {
      emit progress( mChecked, mProcessed );
      mLastChecked = mChecked;
   }
}


void DatabaseWorker::updateFile( const QFileInfo &fileInfo )
{
   ++mChecked;
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
      ++mChecked;
   }
}


bool DatabaseWorker::updateTrackInfoFromFile( const QString &fileName )
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

