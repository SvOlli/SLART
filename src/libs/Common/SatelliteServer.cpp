/**
 * src/libs/Common/SatelliteServer.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 */


#include "SatelliteServer.hpp"

#include <QSignalMapper>


SatelliteServer::SatelliteServer( quint16 port, const QHostAddress &host, QObject *parent )
: QObject( parent )
, mpServer( new QTcpServer( this ) )
, mpClientsReadMapper( new QSignalMapper( this ) )
, mpClientsDisconnectMapper( new QSignalMapper( this ) )
, mClientConnections()
, mPort( port )
, mHost( host )
{
   connect( mpServer, SIGNAL(newConnection()),
            this, SLOT(connected()) );
   connect( mpClientsReadMapper, SIGNAL(mapped(QObject*)),
            this, SLOT(incomingData(QObject*)));
   connect( mpClientsDisconnectMapper, SIGNAL(mapped(QObject*)),
            this, SLOT(disconnected(QObject*)));
}


SatelliteServer::~SatelliteServer()
{
   mpServer->close();
}


bool SatelliteServer::listen()
{
#if SATELLITESERVER_DEBUG
   emit debug( "s:trying to run server on " + 
               mHost.toString().toAscii() + ":" + QByteArray::number(mPort) );
   bool success = mpServer->listen( mHost, mPort );
   
   if( success )
   {
      emit debug( "s:server started successfully" );
   }
   return success;
#else
   return mpServer->listen( mHost, mPort );
#endif
}


void SatelliteServer::incomingData( QObject *client )
{
   QTcpSocket *socket = static_cast<QTcpSocket*>(client);
   QByteArray msg( socket->readAll() );
   
#if SATELLITESERVER_DEBUG
   emit debug( QByteArray("s:from client: ") + msg );
#endif
   for( int i = 0; i < mClientConnections.count(); i++ )
   {
      QTcpSocket *current = mClientConnections.at(i);
      if( current && (client != current) )
      {
         /* make sure messages aren't merged */
         current->waitForBytesWritten( 1000 );
         current->write( msg );
      }
   }
}


void SatelliteServer::connected()
{
   QTcpSocket *socket = mpServer->nextPendingConnection();
   connect(socket, SIGNAL(readyRead()),
           mpClientsReadMapper, SLOT(map()));
   mpClientsReadMapper->setMapping(socket, static_cast<QObject*>(socket));
   connect(socket, SIGNAL(disconnected()),
           mpClientsDisconnectMapper, SLOT(map()));
   mpClientsDisconnectMapper->setMapping(socket, static_cast<QObject*>(socket));
   mClientConnections.append( socket );
#if SATELLITESERVER_DEBUG
   emit debug( QString("s:client connected, %1 clients active")
                  .arg( mClientConnections.count() ).toAscii() );
#endif
}


void SatelliteServer::disconnected( QObject *client )
{
   disconnect(client, SIGNAL(readyRead()),
              mpClientsReadMapper, SLOT(map()));
   mpClientsReadMapper->removeMappings( client );
   disconnect(client, SIGNAL(disconnected()),
              mpClientsDisconnectMapper, SLOT(map()));
   mpClientsDisconnectMapper->removeMappings( client );
   QTcpSocket *socket = static_cast<QTcpSocket*>(client);
   mClientConnections.removeAll( socket );
   socket->deleteLater();
#if SATELLITESERVER_DEBUG
   emit debug( QString("s:client disconnected, %1 clients active")
                  .arg( mClientConnections.count() ).toAscii() );
#endif
}
