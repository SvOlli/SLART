/**
 * Encoder.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#include "Encoder.hpp"

#include "Trace.hpp"
#include <QDir>
#include <QUdpSocket>
#include "MySettings.hpp"

extern "C"
{
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
};

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
      MySettings settings;
      settings.sendNotification( QString("s0d\n") + mFileName );
      if( enqueue )
      {
         settings.sendUdpMessage( QString("P0Q\n") + mFileName, QString("Partyman") );
      }
   }
}
