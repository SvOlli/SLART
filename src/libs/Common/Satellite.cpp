/**
 * src/libs/Common/Satellite.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 */

#include "Satellite.hpp"
#include "SatelliteServerRunner.hpp"
#include "MySettings.hpp"

#include <QtGui>
#include <QDir>

QPointer<Satellite> Satellite::gSatellite = 0;

Satellite::Satellite( QObject *parent )
: QObject( parent )
, mpServerConnection( new QTcpSocket( this ) )
, mpServer( 0 )
, mPort( 0 )
, mHost( QHostAddress::LocalHost )
{
   MySettings settings("Global");
   mHost.setAddress( settings.VALUE_SATELLITE_HOST );
   mPort = settings.VALUE_SATELLITE_PORT;

   connect( mpServerConnection, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(connectFail(QAbstractSocket::SocketError)) );
   connect( mpServerConnection, SIGNAL(disconnected()),
            this, SLOT(disconnected()) );
   connect( mpServerConnection, SIGNAL(readyRead()),
            this, SLOT(incomingData()) );
#if SATELLITE_DEBUG
   connect( mpServerConnection, SIGNAL(connected()),
            this, SLOT(connectSuccess()) );
#endif
}


Satellite::~Satellite()
{
   mpServerConnection->waitForBytesWritten( 1000 );
}


bool Satellite::isRunningServer()
{
   return (mpServer != 0);
}


void Satellite::restart()
{
#if SATELLITE_DEBUG
   emit debug( "c:connecting to server" );
#endif
   if( !MySettings().VALUE_USE_SATELLITE )
   {
      return;
   }

   if( mpServerConnection->state() != QAbstractSocket::UnconnectedState )
   {
      if( (mpServerConnection->peerPort()    != mPort) ||
          (mpServerConnection->peerAddress() != mHost) )
      {
         mpServerConnection->disconnectFromHost();
      }
   }

   if( mpServerConnection->state() == QAbstractSocket::UnconnectedState )
   {
      mpServerConnection->connectToHost( mHost, mPort );
   }
}


void Satellite::send( const QByteArray &message )
{
#if SATELLITE_DEBUG
   emit debug( QByteArray("c:to server: ") + message );
#endif
   int msgSize = message.size();
   mpServerConnection->write( (char*)(&msgSize), sizeof(msgSize) );
   mpServerConnection->write( message );
}


#if SATELLITE_DEBUG
void Satellite::connectSuccess()
{
   emit debug( "c:connected to server in client mode" );
}
#endif


void Satellite::connectFail( QTcpSocket::SocketError socketError )
{
   if( socketError != QTcpSocket::QAbstractSocket::RemoteHostClosedError )
   {
#if SATELLITE_DEBUG
      emit debug( QString("c:connect error %1").arg( socketError ).toAscii() );
#endif
      QTimer::singleShot( 1, this, SLOT(runServer()) );
   }
#if SATELLITE_DEBUG
   emit debug( "c:error connecting to server" );
#endif
   QTimer::singleShot( 1000, this, SLOT(restart()) );
}


void Satellite::runServer()
{
#if SATELLITE_DEBUG
   emit debug( "c:starting server" );
#endif
   if( mpServer == 0 )
   {
      mpServer = new SatelliteServerRunner( mPort, mHost );
      mpServer->start();
      connect( mpServer, SIGNAL(finished()),
               this, SLOT(serverShutdown()) );
#if SATELLITE_DEBUG && SATELLITESERVER_DEBUG
      connect( mpServer, SIGNAL(debug(const QByteArray&)),
               this,     SIGNAL(debug(const QByteArray&)) );
#endif
   }
}


void Satellite::serverShutdown()
{
#if SATELLITE_DEBUG
   emit debug( "c:stopping server (listen failed)" );
#endif
   mpServer = 0;
}


void Satellite::disconnected()
{
#if SATELLITE_DEBUG
   emit debug( "c:disconnected from server" );
#endif
   QTimer::singleShot( 500, this, SLOT(restart()) );
}


void Satellite::incomingData()
{
   int pos = 0;
   QByteArray msg( mpServerConnection->readAll() );

   while( pos < msg.size() )
   {
      int size = *((int*)msg.mid(pos,sizeof(size)).constData());
      pos += sizeof( size );
#if SATELLITE_DEBUG
      emit debug( QByteArray("c:from server: ") + msg.mid(pos,size) );
#endif
      emit received( msg.mid(pos,size) );
      pos += size;
   }
}


void Satellite::send1( const QByteArray &message )
{
   if( gSatellite )
   {
      gSatellite->send( message );
   }
   else
   {
      QTcpSocket socket;
      MySettings settings("Global");

      QHostAddress host( settings.VALUE_SATELLITE_HOST );
      qint16       port( settings.VALUE_SATELLITE_PORT );

      socket.connectToHost( host, port, QIODevice::WriteOnly );
      if( socket.waitForConnected( 1000 ) )
      {
         int msgSize = message.size();
         socket.write( (char*)(&msgSize), sizeof(msgSize) );
         socket.write( message );
         socket.flush();
         socket.disconnectFromHost();
      }
   }
}


Satellite *Satellite::get( QObject *parent )
{
   if( !gSatellite )
   {
      gSatellite = new Satellite( parent );
   }
   return gSatellite;
}


void Satellite::destroy()
{
   if( gSatellite )
   {
      delete gSatellite;
      gSatellite = 0;
   }
}


QStringList Satellite::split( const QByteArray &message )
{
   return QString::fromUtf8( message )
            .remove('\r')
            .split('\n', QString::SkipEmptyParts);
}
