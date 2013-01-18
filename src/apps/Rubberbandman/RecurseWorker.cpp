/*
 * src/apps/Rubberbandman/RecurseWorker.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */


/* class declaration */
#include "RecurseWorker.hpp"

/* system headers */
#include <fileref.h>
#include <tag.h>

/* Qt headers */
#include <QtGui>
#include <QFileInfo>

/* local library headers */
#include <DatabaseInterface.hpp>
#include <DirWalkerCallbackProxy.hpp>
#include <Settings.hpp>

/* local headers */
#include "DirWalkerUpdate.hpp"


RecurseWorker::RecurseWorker()
: QThread( 0 )
, mMode( ModeNothing )
, mpDatabase( DatabaseInterface::get() )
{
   moveToThread( this );
}


RecurseWorker::~RecurseWorker()
{
   if( isRunning() )
   {
      quit();
      wait();
   }
}


void RecurseWorker::startSetTags( const QString &dir, const TrackInfo &ti, Flags flags )
{
   if( mMode != ModeNothing )
   {
      return;
   }

   mMode        = ModeSetTags;
   mTrackInfo   = ti;
   mCancel      = false;
   mSetArtist   = !ti.mArtist.isEmpty();
   mSetTitle    = !ti.mTitle.isEmpty();
   mSetAlbum    = !ti.mAlbum.isEmpty();
   mSetYear     = ti.mYear >= 0;
   mSetGenre    = !ti.mGenre.isEmpty();
   mSetFlags    = ( flags & SetFlags );
   mUnsetFlags  = ( flags & UnsetFlags );
   mSetGroups   = ( flags & SetGroups );
   mUnsetGroups = ( flags & UnsetGroups );
   mDir         = dir;
   if( mSetFlags && mUnsetFlags )
   {
      mSetFlags   = false;
      mUnsetFlags = false;
   }
   if( mSetGroups && mUnsetGroups )
   {
      mSetGroups   = false;
      mUnsetGroups = false;
   }
   start();
}


void RecurseWorker::startNormArtist( const QString &dir )
{
   if( mMode != ModeNothing )
   {
      return;
   }

   mMode        = ModeNormArtist;
   mTrackInfo.clear();
   mCancel      = false;
   mSetArtist   = false;
   mSetTitle    = false;
   mSetAlbum    = false;
   mSetYear     = 0;
   mSetGenre    = false;
   mSetFlags    = false;
   mUnsetFlags  = false;
   mSetGroups   = false;
   mUnsetGroups = false;
   mDir         = dir;
   start();
}


void RecurseWorker::startNormTitle( const QString &dir )
{
   if( mMode != ModeNothing )
   {
      return;
   }

   mMode        = ModeNormTitle;
   mTrackInfo.clear();
   mCancel      = false;
   mSetArtist   = false;
   mSetTitle    = false;
   mSetAlbum    = false;
   mSetYear     = 0;
   mSetGenre    = false;
   mSetFlags    = false;
   mUnsetFlags  = false;
   mSetGroups   = false;
   mUnsetGroups = false;
   mDir         = dir;
   start();
}


void RecurseWorker::run()
{
   mPatternWithoutTrackNr = Settings::value( Settings::RubberbandmanWithoutTrackNr );
   mPatternWithTrackNr    = Settings::value( Settings::RubberbandmanWithTrackNr );
   workDir( mDir );
   mpDatabase->call( this, "done" );
   exec();
}


void RecurseWorker::done()
{
   mMode = ModeNothing;
   quit();
}


void RecurseWorker::workDir( const QString &dir )
{
   QFileInfoList files( QDir(dir).entryInfoList() );

   emit currentPath( dir );
   foreach( const QFileInfo &fileInfo, files )
   {
      if( mCancel )
      {
         break;
      }
      if( fileInfo.fileName().left(1) == "." )
      {
         continue;
      }
      if( fileInfo.isDir() )
      {
         workDir( fileInfo.absoluteFilePath() );
      }
      if( fileInfo.isFile() )
      {
         mpDatabase->getTrackInfo( this, "updateTrackInfo", fileInfo.absoluteFilePath() );
      }
   }
}


void RecurseWorker::updateTrackInfo( const TrackInfo &trackInfo )
{
   bool tagsChanged     = false;
   TrackInfo ti         = trackInfo;
   TrackInfo oldTags;
   QString oldpath( trackInfo.mDirectory + "/" + trackInfo.mFileName );

   mTagMap.clear();
   {
      TagLib::FileRef f( oldpath.toLocal8Bit().data() );
      if( f.file() )
      {
         oldTags = trackInfo;
         if( f.audioProperties() )
         {
            oldTags.mPlayTime = f.audioProperties()->length();
         }

         oldTags.mArtist  = QString::fromUtf8( f.tag()->artist().toCString( true ) );
         oldTags.mTitle   = QString::fromUtf8( f.tag()->title().toCString( true ) );
         oldTags.mAlbum   = QString::fromUtf8( f.tag()->album().toCString( true ) );
         oldTags.mTrackNr = f.tag()->track();
         oldTags.mYear    = f.tag()->year();
         oldTags.mGenre   = QString::fromUtf8( f.tag()->genre().toCString( true ) );
         ti = oldTags;
      }
   }

   switch( mMode )
   {
   case ModeSetTags:
      if( mSetArtist )
      {
         ti.mArtist = mTrackInfo.mArtist;
      }
      if( mSetTitle )
      {
         ti.mTitle.append( " " );
         ti.mTitle.append( mTrackInfo.mTitle );
      }
      if( mSetAlbum )
      {
         ti.mAlbum = mTrackInfo.mAlbum;
      }
      if( mSetYear )
      {
         ti.mYear = mTrackInfo.mYear;
      }
      if( mSetGenre )
      {
         ti.mGenre = mTrackInfo.mGenre;
      }
      tagsChanged = (ti != oldTags);
      if( mSetFlags )
      {
         if( mTrackInfo.isFlagged( TrackInfo::Favorite ) )
         {
            ti.setFlag( TrackInfo::Favorite, true );
         }
         if( mTrackInfo.isFlagged( TrackInfo::Unwanted ) )
         {
            ti.setFlag( TrackInfo::Unwanted, true );
         }
      }
      if( mUnsetFlags )
      {
         if( mTrackInfo.isFlagged( TrackInfo::Favorite ) )
         {
            ti.setFlag( TrackInfo::Favorite, false );
         }
         if( mTrackInfo.isFlagged( TrackInfo::Unwanted ) )
         {
            ti.setFlag( TrackInfo::Unwanted, false );
         }
         if( mTrackInfo.isFlagged( TrackInfo::ScannedWithPower ) ||
             mTrackInfo.isFlagged( TrackInfo::ScannedWithPeak ) )
         {
            ti.setFlag( TrackInfo::ScannedWithPeak, false );
            ti.mLastScanned = 0;
         }
      }
      if( mSetGroups || mUnsetGroups )
      {
         QStringList groups( mTrackInfo.getFolders() );
         foreach( const QString &group, groups )
         {
            ti.setFolder( group, mSetGroups );
         }
      }
      break;
   case ModeNormArtist:
      ti.mArtist = TagMap::normalizeString( oldTags.mArtist );
      tagsChanged = (ti.mArtist != oldTags.mArtist);
      break;
   case ModeNormTitle:
      ti.mTitle = TagMap::normalizeString( oldTags.mTitle );
      tagsChanged = (ti.mTitle != oldTags.mTitle);
      break;
   default:
      break;
   }

   mTagMap.insert( "ARTIST", ti.mArtist );
   mTagMap.insert( "TITLE", ti.mTitle );
   mTagMap.insert( "ALBUM", ti.mAlbum );
   mTagMap.insert( "TRACKNUMBER", (ti.mTrackNr < 0) ? QString() : QString::number(ti.mTrackNr) );
   mTagMap.insert( "DATE", (ti.mYear < 0) ? QString() : QString::number(ti.mYear) );
   mTagMap.insert( "GENRE", ti.mGenre );

   QString newname( mTagMap.fileName( ( ti.mTrackNr < 0 ) ?
                                         mPatternWithoutTrackNr :
                                         mPatternWithTrackNr ) );
   QFileInfo qfi( oldpath );
   QString newpath( qfi.absolutePath() + "/" + newname + "." + qfi.suffix().toLower() );
   QString tmppath( qfi.absolutePath() + "/" + newname + ".rbm." + qfi.suffix().toLower() );
   ti.mFileName = newname + "." + qfi.suffix().toLower();
   mpDatabase->updateTrackInfo( ti );

   if( tagsChanged )
   {
      QFile qf( oldpath );
      if( qf.copy( tmppath ) )
      {
         TagLib::FileRef f( tmppath.toLocal8Bit().data() );
         f.tag()->setArtist( TagLib::String( ti.mArtist.toUtf8().data(), TagLib::String::UTF8 ) );
         f.tag()->setTitle( TagLib::String( ti.mTitle.toUtf8().data(), TagLib::String::UTF8 ) );
         f.tag()->setAlbum( TagLib::String( ti.mAlbum.toUtf8().data(), TagLib::String::UTF8 ) );
         f.tag()->setTrack( ti.mTrackNr );
         f.tag()->setYear( ti.mYear );
         f.tag()->setGenre( TagLib::String( ti.mGenre.toUtf8().data(), TagLib::String::UTF8 ) );
         f.save();
         QFile( oldpath ).remove();
         QFile::rename( tmppath, newpath );
      }
   }
   else
   {
      if( oldpath != newpath )
      {
         QFile::rename( oldpath, newpath );
      }
   }
}
