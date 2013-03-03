/*
 * src/apps/Rubberbandman/DatabaseWorker.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
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
#include <DirWalker.hpp>

/* local headers */
#include "DirWalkerUpdate.hpp"


static const int UPDATE_TIMEOUT_MS = 333;


DatabaseWorker::DatabaseWorker( QObject *parent )
: QObject( parent )
, mpDatabase( DatabaseInterface::get() )
, mChecked( 0 )
, mProcessed( 0 )
, mTime()
{
}


DatabaseWorker::~DatabaseWorker()
{
}


void DatabaseWorker::setPath( const QString &path )
{
   mPath = path;
}


void DatabaseWorker::updateStatus()
{
   emit progress( mChecked, mProcessed );
}


void DatabaseWorker::startUpdate()
{
   mChecked   = 0;
   mProcessed = 0;

   emit message( tr("updating %1").arg( mPath ) );
   emit progress( mChecked, mProcessed );
   DirWalkerUpdate walkerCallbacks( this );
   DirWalker().run( &walkerCallbacks, mPath );
   mpDatabase->call( this, SIGNAL(done()) );
}


void DatabaseWorker::startCleanup()
{
   mChecked   = 0;
   mProcessed = 0;

   emit message( tr("cleaning up") );
   emit progress( mChecked, mProcessed );
   mpDatabase->getTrackInfoList( this, SLOT(cleanup(TrackInfoList)) );
}


void DatabaseWorker::startImport()
{
   mChecked   = 0;
   mProcessed = 0;

   emit message( tr("importing %1").arg( mPath ) );
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
      fileName = QString::fromLocal8Bit( m3uFile.readLine().constData() );
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


void DatabaseWorker::cleanup( const TrackInfoList &trackInfoList )
{
   QFileInfo qfi;
   foreach( TrackInfo trackInfo, trackInfoList )
   {
      qfi.setFile( trackInfo.mDirectory + "/" + trackInfo.mFileName );
      if( !qfi.isFile() )
      {
         mpDatabase->deleteTrackInfo( trackInfo );
         ++mProcessed;
      }
      ++mChecked;
      if( mTime.elapsed() > UPDATE_TIMEOUT_MS )
      {
         emit progress( mChecked, mProcessed );
         mTime.restart();
      }
   }
   emit progress( mChecked, mProcessed );
   mpDatabase->call( this, SIGNAL(done()) );
}


void DatabaseWorker::handleFile( const QFileInfo &fileInfo )
{
   mpDatabase->getTrackInfo( this, SLOT(updateTrackInfoFromFile(TrackInfo)), fileInfo.absoluteFilePath() );
}


void DatabaseWorker::handleEnd()
{
   emit progress( mChecked, mProcessed );
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
   ++mChecked;

   if( mTime.elapsed() > UPDATE_TIMEOUT_MS )
   {
      emit progress( mChecked, mProcessed );
      mTime.restart();
   }
}
