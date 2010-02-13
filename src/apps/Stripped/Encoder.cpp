/**
 * Encoder.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#include "Encoder.hpp"

#include <QDir>
#include <QUdpSocket>

extern "C"
{
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
}

#include "MySettings.hpp"
#include "Satellite.hpp"

#include "Trace.hpp"

Encoder::Encoder( QWidget *parent, const QString &encoderName )
: QWidget( parent )
, name( encoderName )
, mFD( -1 )
{
}


Encoder::~Encoder()
{
}


void Encoder::initialize( const QString &fileName, const char *extension )
{
   QFileInfo qfi( fileName+extension );
   QDir dir( qfi.absoluteDir() );
   
   if( !dir.exists() )
   {
      dir.mkpath( dir.absolutePath() );
   }

   mFileName = qfi.absoluteFilePath();
   mFD = ::open( mFileName.toLocal8Bit().data(), O_WRONLY | O_CREAT | O_TRUNC, 0644 );
}


void Encoder::finalize( bool enqueue, bool cancel )
{
   ::close( mFD );
   mFD = -1;
   
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
}
