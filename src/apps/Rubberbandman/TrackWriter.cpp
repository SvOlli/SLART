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


static const char *signature_mismatch_msg = "%s:%d %s does not match %s or %s in %s";
static const char *call_failed_msg = "%s:%d calling %s failed in %s";

static inline QByteArray slotName( const char *method )
{
   const char *c = 0;
   for( c = method + 1; *c; ++c )
   {
      if( *c == '(' )
      {
         break;
      }
   }
   return QByteArray( method + 1, c - method - 1 );
}


TrackWriter::TrackWriter( QObject *target, const char *method,
                          enum Qt::ConnectionType type )
: QObject( 0 )
, mpTarget( target )
, mpMethod( method )
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
   if( Settings::value( Settings::RubberbandmanAutoRename ) && (newName != trackInfo.mFileName) )
   {
      QFile::rename( trackInfo.mDirectory + "/" + trackInfo.mFileName,
                     trackInfo.mDirectory + "/" + newName );
      ti.mFileName = newName;
   }
   DatabaseInterface *database = DatabaseInterface::get();
   ti.mLastTagsRead = QFileInfo( ti.mDirectory + "/" + ti.mFileName ).lastModified().toTime_t();
   database->updateTrackInfo( ti, true );

   if( mpTarget && !mpMethod )
   {
      if( ti.mID > 0 )
      {
         const char *signal1 = SIGNAL(signal(TrackInfo));
         if( QMetaObject::checkConnectArgs( signal1, mpMethod ) )
         {
            if( !QMetaObject::invokeMethod( mpTarget, slotName( mpMethod ).constData(),
                                            mType, Q_ARG( TrackInfo, ti ) ) )
            {
               qFatal( call_failed_msg, __FILE__, __LINE__, signal1, Q_FUNC_INFO );
            }
         }
         else
         {
            qFatal( signature_mismatch_msg, __FILE__, __LINE__, signal1, mpMethod, Q_FUNC_INFO );
         }
      }
      else
      {
         database->getTrackInfo( mpTarget, mpMethod,
                                 ti.mDirectory + "/" + ti.mFileName );
      }
   }

   deleteLater();
}
