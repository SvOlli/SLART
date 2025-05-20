/*
 * src/libs/Common/Satellite.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

/* class declaration */
#define SATELLITE_PKG_HEADER SATELLITE_PKG_HEADER
#include "Satellite.hpp"

/* system headers */

/* Qt headers */
#include <QDir>
#include <QTcpSocket>
#include <QTimer>

/* local library headers */

/* local headers */
#include "SatelliteServer.hpp"

/* class variable instantiation */
Satellite *Satellite::cpSatellite = 0;


Satellite *Satellite::create( const QHostAddress &host, quint16 port )
{
   if( !cpSatellite )
   {
      cpSatellite = new Satellite( host, port );
      // start() needs to be run after whole system has been set up
   }
   return cpSatellite;
}


Satellite *Satellite::get()
{
   return cpSatellite;
}


void Satellite::destroy()
{
   if( cpSatellite )
   {
      cpSatellite->quit();
      cpSatellite->wait();
      delete cpSatellite;
      cpSatellite = 0;
   }
}


Satellite::Satellite( const QHostAddress &host, quint16 port )
: QThread( 0 )
, mpServerConnection( 0 )
, mpServer( 0 )
, mHost( host )
, mPort( port )
{
   setObjectName( "SatelliteThread" );
   moveToThread( this );
}


Satellite::~Satellite()
{
}


void Satellite::setHostPort( const QHostAddress &host, quint16 port )
{
   mHost = host;
   mPort = port;
   if( mpServerConnection->state() != QAbstractSocket::UnconnectedState )
   {
      restart();
   }
}


void Satellite::run()
{
   if( !mpServerConnection )
   {
      mpServerConnection = new QTcpSocket( this );
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
      connect( mpServerConnection, SIGNAL(connected()),
               this, SIGNAL(connected()) );
   }
#if SATELLITE_DEBUG
   emit debug( "c:entering event handler" );
#endif
   restart();
   exec();
   if( mpServerConnection->state() != QTcpSocket::UnconnectedState )
   {
      mpServerConnection->waitForBytesWritten( 1000 );
   }
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
      if( mpServer )
      {
         mpServer->deleteLater();
      }

      mpServerConnection->connectToHost( mHost, mPort, QIODevice::ReadWrite | QIODevice::Unbuffered );
   }
}


void Satellite::send( const QByteArray &message )
{
   qint64 bytesSent;
#if SATELLITE_DEBUG
   emit debug( QByteArray("c:to server: ") + message );
#endif
   SATELLITE_PKGINFO_HEADER_TYPE   header( SATELLITE_PKGINFO_MAGIC_VALUE );
   header <<= 32;
   header |= message.size();
   header = qToBigEndian( header );
   SATELLITE_PKGINFO_CHECKSUM_TYPE checksum( qChecksum( message.constData(), message.size() ) );
   checksum = qToBigEndian( checksum );
   bytesSent = mpServerConnection->write( (char*)(&header), SATELLITE_PKGINFO_HEADER_SIZE );
   if( bytesSent < 0 )
   {
      qWarning( "bytesSent < 0" );
   }
   bytesSent = mpServerConnection->write( message );
   if( bytesSent < 0 )
   {
      qWarning( "bytesSent < 0" );
   }
   bytesSent = mpServerConnection->write( (char*)(&checksum), SATELLITE_PKGINFO_CHECKSUM_SIZE );
   if( bytesSent < 0 )
   {
      qWarning( "bytesSent < 0" );
   }
   mpServerConnection->flush();
   mpServerConnection->waitForBytesWritten( 10000 );
}


#if SATELLITE_DEBUG
void Satellite::connectSuccess()
{
   emit debug( "c:connected to server in client mode" );
}
#endif


void Satellite::connectFail( QAbstractSocket::SocketError socketError )
{
   if( socketError != QAbstractSocket::RemoteHostClosedError )
   {
#if SATELLITE_DEBUG
      emit debug( QString("c:connect error %1").arg( socketError ).toLatin1() );
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
   if( mpServer == 0 )
   {
#if SATELLITE_DEBUG
      emit debug( "c:starting server" );
#endif
      SatelliteServer *mpServer = new SatelliteServer( this );
#if SATELLITESERVER_DEBUG
      connect( mpServer, SIGNAL(debug(QByteArray)),
               this,     SIGNAL(debug(QByteArray)) );
#endif
      if( !mpServer->listen( mHost, mPort ) )
      {
         mpServer->deleteLater();
      }
   }
}


#if SATELLITE_DEBUG
void Satellite::serverShutdown()
{
   emit debug( "c:stopping server (listen failed)" );
}
#endif


void Satellite::disconnected()
{
#if SATELLITE_DEBUG
   emit debug( "c:disconnected from server" );
#endif
   QTimer::singleShot( 500, this, SLOT(restart()) );
}


void Satellite::incomingData()
{
   SATELLITE_PKGINFO_HEADER_TYPE   header( 0 );
   SATELLITE_PKGINFO_CHECKSUM_TYPE checksum( 0 );
   while( mpServerConnection->bytesAvailable() > SATELLITE_PKGINFO_HEADER_SIZE )
   {
      mpServerConnection->peek( (char*)(&header), SATELLITE_PKGINFO_HEADER_SIZE );
      header = qFromBigEndian( header );
      if( (header >> 32) != SATELLITE_PKGINFO_MAGIC_VALUE )
      {
         /* invalid data, flush */
         mpServerConnection->readAll();
         break;
      }
      if( mpServerConnection->bytesAvailable() <
            ((qint32)(header & 0xFFFFFFFF) + SATELLITE_PKGINFO_HEADER_SIZE) )
      {
         /* buffer incomplete, let the try at next signal */
         break;
      }
      mpServerConnection->read( (char*)(&header), SATELLITE_PKGINFO_HEADER_SIZE );
      header = qFromBigEndian( header );
      QByteArray msg( mpServerConnection->read( header & 0xFFFFFFFF ) );
      mpServerConnection->read( (char*)(&checksum), SATELLITE_PKGINFO_CHECKSUM_SIZE );
      checksum = qFromBigEndian( checksum );
#if SATELLITE_DEBUG
      emit debug( QByteArray("c:from server: ") + msg );
#endif
      if( checksum == qChecksum( msg.constData(), header & 0xFFFFFFFF ) )
      {
         emit received( msg );
      }
      else
      {
         /* checksum mismatch, an error occurred, flush all buffers */
         mpServerConnection->readAll();
      }
   }
}


QStringList Satellite::split( const QByteArray &message )
{
   return QString::fromUtf8( message.constData() )
            .remove('\r')
            .split('\n', Qt::SkipEmptyParts);
}
