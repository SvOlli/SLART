/**
 * src/apps/Rubberbandman/DatabaseWorker.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */


/* class declaration */
#include "DatabaseWorker.hpp"

/* system headers */
#include <fileref.h>
#include <tag.h>

/* Qt headers */
#include <QtGui>
#include <QFileInfo>

/* local library headers */
#include <DatabaseInterface.hpp>
#include <DirWalkerCallbackProxy.hpp>

/* local headers */
#include "DirWalkerUpdate.hpp"


static const int UPDATE_INCREMENT = 200;


DatabaseWorker::DatabaseWorker()
: QThread( 0 )
, mMode( NONE )
, mpDatabase( DatabaseInterface::get() )
{
   moveToThread( this );
}


DatabaseWorker::~DatabaseWorker()
{
   if( isRunning() )
   {
      quit();
      wait();
   }
}


bool DatabaseWorker::initUpdate( const QString &baseDir )
{
   if( mMode != NONE )
   {
      return false;
   }
   mMode   = UPDATE;
   mCancel = false;
   mPath   = baseDir;

   return true;
}


bool DatabaseWorker::initCleanup()
{
   if( mMode != NONE )
   {
      return false;
   }
   mMode   = CLEANUP;
   mCancel = false;

   return true;
}


bool DatabaseWorker::initImport( const QString &fileName )
{
   if( mMode != NONE )
   {
      return false;
   }
   mMode   = IMPORT;
   mCancel = false;
   mPath   = fileName;

   return true;
}


void DatabaseWorker::run()
{
   mChecked     = 0;
   mLastChecked = 0;
   mProcessed   = 0;
   emit progress( mChecked, mProcessed );
   switch( mMode )
   {
      case UPDATE:
         {
            DirWalkerUpdate walkerCallbacks( this );
            mDirWalker.run( &walkerCallbacks, mPath );
         }
         exec();
         break;
      case CLEANUP:
         mpDatabase->getTrackInfoList( this, "cleanup" );
         exec();
         break;
      case IMPORT:
         importM3u();
         break;
      default:
         break;
   }
   emit progress( mChecked, mProcessed );
   mMode = NONE;
}


void DatabaseWorker::cleanup( const TrackInfoList &trackInfoList )
{
   QFileInfo qfi;
   foreach( mTrackInfo, trackInfoList )
   {
      qfi.setFile( mTrackInfo.mDirectory + "/" + mTrackInfo.mFileName );
      if( !qfi.isFile() )
      {
         mpDatabase->deleteTrackInfo( mTrackInfo );
         ++mProcessed;
      }
      if( ++mChecked >= mLastChecked + UPDATE_INCREMENT )
      {
         emit progress( mChecked, mProcessed );
         mLastChecked = mChecked;
      }
   }
   emit progress( mChecked, mProcessed );
   //mpDatabase->cleanup(); //TODO
   mpDatabase->call( this, "quit" );
}


void DatabaseWorker::handleFile( const QFileInfo &fileInfo )
{
   mpDatabase->getTrackInfo( this, "updateTrackInfoFromFile", fileInfo.absoluteFilePath() );
}


void DatabaseWorker::handleEnd()
{
   mpDatabase->call( this, "quit" );
}


void DatabaseWorker::updateTrackInfoFromFile( const TrackInfo &trackInfo )
{
   QFileInfo fileInfo( trackInfo.mDirectory + "/" + trackInfo.mFileName );

   if( (fileInfo.lastModified().toTime_t() > trackInfo.mLastTagsRead ) ||
       (trackInfo.mPlayTime == 0) )
   {
      TagLib::FileRef f( fileInfo.absoluteFilePath().toLocal8Bit().data() );
      if( f.file() && f.tag() )
      {
         TrackInfo ti( trackInfo );
         ti.mArtist       = QString::fromUtf8( f.tag()->artist().toCString( true ) );
         ti.mTitle        = QString::fromUtf8( f.tag()->title().toCString( true ) );
         ti.mAlbum        = QString::fromUtf8( f.tag()->album().toCString( true ) );
         ti.mTrackNr      = f.tag()->track();
         ti.mYear         = f.tag()->year();
         ti.mGenre        = QString::fromUtf8( f.tag()->genre().toCString( true ) );
         if( f.audioProperties() )
         {
            ti.mPlayTime  = f.audioProperties()->length();
         }
         ti.mLastTagsRead = fileInfo.lastModified().toTime_t();

         mpDatabase->updateTrackInfo( ti, true );
         ++mProcessed;
      }
   }

   if( ++mChecked >= mLastChecked + UPDATE_INCREMENT )
   {
      emit progress( mChecked, mProcessed );
      mLastChecked = mChecked;
   }
}


void DatabaseWorker::importM3u()
{
   QFile m3uFile( mPath );
   if( !m3uFile.exists() )
   {
      return;
   }
   if( !m3uFile.open( QIODevice::ReadOnly | QIODevice::Text ) )
   {
      return;
   }

   QString fileName;
   QString fileBase( mPath + "/../" );
   QFileInfo fileInfo;
   TrackInfo ti;
   while( !m3uFile.atEnd() )
   {
      fileName = QString::fromLocal8Bit( m3uFile.readLine() );
      if( !fileName.startsWith("#") )
      {
         if( fileName.right(1) == QChar('\n') )
         {
            fileName.chop(1);
         }
         if( fileName.startsWith( "/" ) )
         {
            fileInfo.setFile( fileName );
         }
         else
         {
            /* a bit of an ugly trick, but gets the job done better than most
               other solutions */
            fileInfo.setFile( fileBase + fileName );
            fileName = fileInfo.absoluteFilePath();
         }
         ti.clear();
         ti.mDirectory = fileInfo.absolutePath();
         ti.mFileName  = fileInfo.fileName();
         TagLib::FileRef f( fileInfo.absoluteFilePath().toLocal8Bit().data() );
         if( f.file() && f.tag() )
         {
            ti.mArtist       = QString::fromUtf8( f.tag()->artist().toCString( true ) );
            ti.mTitle        = QString::fromUtf8( f.tag()->title().toCString( true ) );
            ti.mAlbum        = QString::fromUtf8( f.tag()->album().toCString( true ) );
            ti.mTrackNr      = f.tag()->track();
            ti.mYear         = f.tag()->year();
            ti.mGenre        = QString::fromUtf8( f.tag()->genre().toCString( true ) );
            if( f.audioProperties() )
            {
               ti.mPlayTime  = f.audioProperties()->length();
            }
            ti.mLastTagsRead = fileInfo.lastModified().toTime_t();
         }
         mpDatabase->updateTrackInfo( ti, true );
      }
   }
   m3uFile.close();
}
