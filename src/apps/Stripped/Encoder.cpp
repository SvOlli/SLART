/**
 * src/apps/Stripped/Encoder.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#include "Encoder.hpp"

#include <QDir>

#include "MySettings.hpp"
#include "Satellite.hpp"

#include "Trace.hpp"

Encoder::Encoder( QWidget *parent, const QString &encoderName )
: QWidget( parent )
, mName( encoderName )
, mFile()
{
}


Encoder::~Encoder()
{
}


bool Encoder::initialize( const QString &fileName, const char *extension )
{
   QFileInfo qfi( fileName+extension );
   QDir dir( qfi.absoluteDir() );
   
   if( !dir.exists() )
   {
      if( !dir.mkpath( dir.absolutePath() ) )
      {
         return false;
      }
   }

   mFileName = qfi.absoluteFilePath();
   mFile.setFileName( mFileName );
   return mFile.open( QIODevice::WriteOnly );
}


bool Encoder::finalize( bool enqueue, bool cancel )
{
   mFile.close();
   
   if( !cancel )
   {
      Satellite *satellite = Satellite::get();
      QByteArray msg( "s0d\n" );
      msg.append( mFileName.toUtf8() );
      satellite->send( msg );
      if( enqueue )
      {
         msg = "P0Q\n";
         msg.append( mFileName.toUtf8() );
         satellite->send( msg );
      }
   }
   return !cancel;
}


bool Encoder::writeChunk( const char* buffer, qint64 size )
{
   while( size > 0 )
   {
      qint64 written = mFile.write( buffer, size );
      if( written < 0 )
      {
         return false;
      }
      size   -= written;
      buffer += written;
   }
   return true;
}
