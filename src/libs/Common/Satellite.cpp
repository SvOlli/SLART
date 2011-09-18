/**
 * src/libs/Common/Satellite.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

/* class declaration */
#include "Satellite.hpp"

/* system headers */

/* Qt headers */
#include <QtGui>
#include <QDir>

/* local library headers */

/* local headers */
#include "SatelliteServerRunner.hpp"
#include "MySettings.hpp"

/* class variable instantiation */
Satellite *Satellite::cpSatellite = 0;


Satellite *Satellite::create()
{
   if( !cpSatellite )
   {
      cpSatellite = new Satellite();
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
      delete cpSatellite;
      cpSatellite = 0;
   }
}


Satellite::Satellite( QObject *parent )
: QObject( parent )
, mIsTestApp( false )
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
   if( mpServerConnection->state() != QTcpSocket::UnconnectedState )
   {
      mpServerConnection->waitForBytesWritten( 1000 );
   }
}


void Satellite::setTestApp( bool isTestApp )
{
   mIsTestApp = isTestApp;
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
   if( !mIsTestApp && !MySettings().VALUE_USE_SATELLITE )
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
      connect( mpServerConnection, SIGNAL(connected()),
               this, SIGNAL(connected()) );
   }
}


bool Satellite::waitForConnected( int msecs )
{
   return mpServerConnection->waitForConnected( msecs );
}


void Satellite::send( const QByteArray &message )
{
#if SATELLITE_DEBUG
   emit debug( QByteArray("c:to server: ") + message );
#endif
   SATELLITE_PKGINFO_HEADER_TYPE   header( SATELLITE_PKGINFO_MAGIC_VALUE );
   header <<= 32;
   header |= message.size();
   header = qToBigEndian( header );
   SATELLITE_PKGINFO_CHECKSUM_TYPE checksum( qChecksum( message.constData(), message.size() ) );
   checksum = qToBigEndian( checksum );
   mpServerConnection->write( (char*)(&header), SATELLITE_PKGINFO_HEADER_SIZE );
   mpServerConnection->write( message );
   mpServerConnection->write( (char*)(&checksum), SATELLITE_PKGINFO_CHECKSUM_SIZE );
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
#if SATELLITE_DEBUG
      connect( mpServer, SIGNAL(finished()),
               this, SLOT(serverShutdown()) );
#if SATELLITESERVER_DEBUG
      connect( mpServer, SIGNAL(debug(QByteArray)),
               this,     SIGNAL(debug(QByteArray)) );
#endif
#endif
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


void Satellite::send1( const QByteArray &message )
{
   if( cpSatellite )
   {
      cpSatellite->send( message );
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
         SATELLITE_PKGINFO_HEADER_TYPE   header( SATELLITE_PKGINFO_MAGIC_VALUE );
         header <<= 32;
         header |= message.size();
         header = qToBigEndian( header );
         SATELLITE_PKGINFO_CHECKSUM_TYPE checksum( qChecksum( message.constData(), message.size() ) );
         checksum = qToBigEndian( checksum );
         socket.write( (char*)(&header), SATELLITE_PKGINFO_HEADER_SIZE );
         socket.write( message );
         socket.write( (char*)(&checksum), SATELLITE_PKGINFO_CHECKSUM_SIZE );
         socket.flush();
         socket.disconnectFromHost();
      }
   }
}


QStringList Satellite::split( const QByteArray &message )
{
   return QString::fromUtf8( message.constData() )
            .remove('\r')
            .split('\n', QString::SkipEmptyParts);
}
