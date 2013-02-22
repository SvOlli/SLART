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


static const char *signature_mismatch_msg = "%s:%d %s does not match %s in %s";
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


TrackReader::TrackReader( QObject *target, const char *method,
                          enum Qt::ConnectionType type )
: QObject( 0 )
, mpTarget( target )
, mpMethod( method )
, mType( type )
{
}


TrackReader::~TrackReader()
{
}


void TrackReader::read( const QString &fileName )
{
   DatabaseInterface::get()->getTrackInfo( this, SLOT(handleTrackInfo(TrackInfo)), fileName );
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

   deleteLater();
}
