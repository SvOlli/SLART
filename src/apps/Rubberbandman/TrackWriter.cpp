/*
 * src/libs/Common/TrackWriter.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

/* class declaration */
#include "TrackWriter.hpp"

/* system headers */
#include <fileref.h>
#include <tag.h>

/* Qt headers */
#include <QtGui>

/* local library headers */
#include <DatabaseInterface.hpp>
#include <Settings.hpp>
#include <TagMap.hpp>
#include <TrackInfo.hpp>

/* local headers */


TrackWriter::TrackWriter( QObject *target, const QString &method,
                          enum Qt::ConnectionType type )
: QObject( 0 )
, mpTarget( target )
, mMethod( method )
, mType( type )
{
}


TrackWriter::~TrackWriter()
{
}


void TrackWriter::write( const TrackInfo &trackInfo )
{
   QString fileName = trackInfo.mDirectory + "/" + trackInfo.mFileName;
   TagLib::FileRef f( fileName.toLocal8Bit().data() );
   if( f.file() )
   {
      bool update = false;
      if( trackInfo.mArtist != QString::fromUtf8( f.tag()->artist().toCString( true ) ) )
      {
         update = true;
      }
      else if( trackInfo.mTitle != QString::fromUtf8( f.tag()->title().toCString( true ) ) )
      {
         update = true;
      }
      else if( trackInfo.mAlbum != QString::fromUtf8( f.tag()->album().toCString( true ) ) )
      {
         update = true;
      }
      else if( trackInfo.mTrackNr != f.tag()->track() )
      {
         update = true;
      }
      else if( trackInfo.mYear != f.tag()->year() )
      {
         update = true;
      }
      else if( trackInfo.mGenre != QString::fromUtf8( f.tag()->genre().toCString( true ) ) )
      {
         update = true;
      }
      if( update )
      {
         f.tag()->setArtist( TagLib::String( trackInfo.mArtist.toUtf8().data(), TagLib::String::UTF8 ) );
         f.tag()->setTitle( TagLib::String( trackInfo.mTitle.toUtf8().data(), TagLib::String::UTF8 ) );
         f.tag()->setAlbum( TagLib::String( trackInfo.mAlbum.toUtf8().data(), TagLib::String::UTF8 ) );
         f.tag()->setTrack( trackInfo.mTrackNr );
         f.tag()->setYear( trackInfo.mYear );
         f.tag()->setGenre( TagLib::String( trackInfo.mGenre.toUtf8().data(), TagLib::String::UTF8 ) );
         f.save();
      }
   }
   TagMap tagMap;
   tagMap.insert( "ARTIST", trackInfo.mArtist );
   tagMap.insert( "TITLE", trackInfo.mTitle );
   tagMap.insert( "ALBUM", trackInfo.mAlbum );
   tagMap.insert( "TRACKNUMBER", (trackInfo.mTrackNr < 0) ? QString() : QString::number(trackInfo.mTrackNr) );
   tagMap.insert( "DATE", (trackInfo.mYear < 0) ? QString() : QString::number(trackInfo.mYear) );
   tagMap.insert( "GENRE", trackInfo.mGenre );

   QString newName;
   if( trackInfo.mTrackNr < 0 )
   {
      newName = tagMap.fileName( Settings::value( Settings::RubberbandmanWithoutTrackNr ) );
   }
   else
   {
      newName = tagMap.fileName( Settings::value( Settings::RubberbandmanWithTrackNr ) );
   }
   newName.append( trackInfo.mFileName.mid( trackInfo.mFileName.lastIndexOf('.') ) );
   TrackInfo ti( trackInfo );
   if( !Settings::value( Settings::RubberbandmanAutoRename ) && (newName != trackInfo.mFileName) )
   {
      QFile::rename( trackInfo.mDirectory + "/" + trackInfo.mFileName,
                     trackInfo.mDirectory + "/" + newName );
      ti.mFileName = newName;
   }
   DatabaseInterface *database = DatabaseInterface::get();
   ti.mLastTagsRead = QFileInfo( ti.mDirectory + "/" + ti.mFileName ).lastModified().toTime_t();
   database->updateTrackInfo( ti, true );

   if( mpTarget && !mMethod.isEmpty() )
   {
      if( ti.mID > 0 )
      {
         if( !QMetaObject::invokeMethod( mpTarget, mMethod.toAscii().constData(),
                                         mType, Q_ARG( TrackInfo, ti ) ) )
         {
            qFatal( "%s:%d call failed in %s", __FILE__, __LINE__, Q_FUNC_INFO );
         }
      }
      else
      {
         database->getTrackInfo( mpTarget, mMethod,
                                 ti.mDirectory + "/" + ti.mFileName );
      }
   }

   deleteLater();
}
