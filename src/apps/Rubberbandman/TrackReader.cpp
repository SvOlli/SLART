/*
 * src/libs/Common/TrackReader.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

/* class declaration */
#include "TrackReader.hpp"

/* system headers */
#include <fileref.h>
#include <tag.h>

/* Qt headers */
#include <QtGui>

/* local library headers */
#include <DatabaseInterface.hpp>
#include <TrackInfo.hpp>

/* local headers */


TrackReader::TrackReader( QObject *target, const QString &method,
                          enum Qt::ConnectionType type )
: QObject( 0 )
, mpTarget( target )
, mMethod( method )
, mType( type )
{
}


TrackReader::~TrackReader()
{
}


void TrackReader::read( const QString &fileName )
{
   DatabaseInterface::get()->getTrackInfo( this, "handleTrackInfo", fileName );
}


void TrackReader::handleTrackInfo( const TrackInfo &trackInfo )
{
   TrackInfo ti( trackInfo );
   QString fileName( QString("%1/%2").arg( trackInfo.mDirectory, trackInfo.mFileName ) );

   /* read tags into TrackInfo */
   {
      TagLib::FileRef f( fileName.toLocal8Bit().data() );
      if( f.file() )
      {
         if( f.audioProperties() )
         {
            ti.mPlayTime = f.audioProperties()->length();
         }
         ti.mArtist  = QString::fromUtf8( f.tag()->artist().toCString( true ) );
         ti.mTitle   = QString::fromUtf8( f.tag()->title().toCString( true ) );
         ti.mAlbum   = QString::fromUtf8( f.tag()->album().toCString( true ) );
         ti.mTrackNr = f.tag()->track();
         ti.mYear    = f.tag()->year();
         ti.mGenre   = QString::fromUtf8( f.tag()->genre().toCString( true ) );
      }
   }

   if( !QMetaObject::invokeMethod( mpTarget, mMethod.toAscii().constData(),
                                   mType, Q_ARG( TrackInfo, ti ) ) )
   {
      qFatal( "%s:%d call failed in %s", __FILE__, __LINE__, Q_FUNC_INFO );
   }

   deleteLater();
}
