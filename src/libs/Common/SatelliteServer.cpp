/**
 * src/libs/Common/SatelliteServer.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

/* class declaration */
#include "SatelliteServer.hpp"

/* system headers */

/* Qt headers */
#include <QSignalMapper>
#include <QTcpServer>
#include <QTcpSocket>

/* local library headers */

/* local headers */
#define SATELLITE_PKG_HEADER SATELLITE_PKG_HEADER
#include "Satellite.hpp"


SatelliteServer::SatelliteServer( QObject *parent )
: QObject( parent )
, mpTcpServer( new QTcpServer( this ) )
, mpClientsReadMapper( new QSignalMapper( this ) )
, mpClientsDisconnectMapper( new QSignalMapper( this ) )
, mClientConnections()
{
   connect( mpTcpServer, SIGNAL(newConnection()),
            this, SLOT(connected()) );
   connect( mpClientsReadMapper, SIGNAL(mapped(QObject*)),
            this, SLOT(incomingData(QObject*)));
   connect( mpClientsDisconnectMapper, SIGNAL(mapped(QObject*)),
            this, SLOT(disconnected(QObject*)));
}


SatelliteServer::~SatelliteServer()
{
   mpTcpServer->close();
}


bool SatelliteServer::listen( const QHostAddress &host, quint16 port )
{
#if SATELLITESERVER_DEBUG
   emit debug( "s:trying to run server on " +
               mHost.toString().toAscii() + ":" + QByteArray::number(Satellite::port()) );
   bool success = mpTcpServer->listen( host, port );

   if( success )
   {
      emit debug( "s:server started successfully" );
   }
   return success;
#else
   return mpTcpServer->listen( host, port );
#endif
}


void SatelliteServer::connected()
{
   QTcpSocket *socket = mpTcpServer->nextPendingConnection();
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


void SatelliteServer::incomingData( QObject *client )
{
   QTcpSocket *socket = static_cast<QTcpSocket*>(client);

   while( socket->bytesAvailable() > SATELLITE_PKGINFO_HEADER_SIZE )
   {
      SATELLITE_PKGINFO_HEADER_TYPE   header( 0 );

      socket->peek( (char*)(&header), SATELLITE_PKGINFO_HEADER_SIZE );
      header = qFromBigEndian( header );
      if( (header >> 32) != SATELLITE_PKGINFO_MAGIC_VALUE )
      {
         /* invalid data, flush */
         socket->readAll();
#if SATELLITESERVER_DEBUG
         emit debug( "s:got bad data, flushing" );
#endif
         break;
      }
      qint64 datasize = (header & 0xFFFFFFFF) +
         SATELLITE_PKGINFO_CHECKSUM_SIZE + SATELLITE_PKGINFO_HEADER_SIZE;
      if( socket->bytesAvailable() < datasize )
      {
         /* buffer incomplete, let the try at next signal */
#if SATELLITESERVER_DEBUG
         emit debug( "s:got incomplete data" );
#endif
         break;
      }
      QByteArray msg( socket->read( datasize ) );

#if SATELLITESERVER_DEBUG
      /* for debugging show raw message as hex dump */
      emit debug( QByteArray("s:from client: ") + msg.toHex() );
#endif
      for( int i = 0; i < mClientConnections.count(); i++ )
      {
         QTcpSocket *current = mClientConnections.at(i);
         if( current && (client != current) )
         {
            current->write( msg );
         }
      }
   }
}
