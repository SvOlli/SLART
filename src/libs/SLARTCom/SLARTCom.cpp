/**
 * src/libs/SLARTCom/SLARTCom.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 */

#include "SLARTCom.hpp"
#include "SLARTComServerRunner.hpp"

#include <QtGui>
#include <QDir>

SLARTCom::SLARTCom( quint16 port, QObject *parent )
: QObject( parent )
, mpServerConnection( new QTcpSocket( this ) )
, mpServer( 0 )
, mPort( port )
, mHost( QHostAddress::LocalHost )
{
   connect( mpServerConnection, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(connectFail(QAbstractSocket::SocketError)) );
   connect( mpServerConnection, SIGNAL(disconnected()),
            this, SLOT(disconnected()) );
   connect( mpServerConnection, SIGNAL(readyRead()),
            this, SLOT(incomingData()) );
#if SLARTCOM_DEBUG
   connect( mpServerConnection, SIGNAL(connected()),
            this, SLOT(connectSuccess()) );
#endif
}


SLARTCom::~SLARTCom()
{
}


void SLARTCom::setHost( const QHostAddress &host )
{
   mHost = host;
}


bool SLARTCom::isRunningServer()
{
   return (mpServer != 0);
}


void SLARTCom::start()
{
#if SLARTCOM_DEBUG
   emit debug( "c:connecting to server" );
#endif
   if( mpServerConnection->state() == QAbstractSocket::UnconnectedState )
   {
      mpServerConnection->connectToHost( mHost, mPort );
   }
}


void SLARTCom::send( const QByteArray &message )
{
#if SLARTCOM_DEBUG
   emit debug( QByteArray("c:to server: ") + message );
#endif
   mpServerConnection->write( message );
}


#if SLARTCOM_DEBUG
void SLARTCom::connectSuccess()
{
   emit debug( "c:connected to server in client mode" );
}
#endif


void SLARTCom::connectFail( QTcpSocket::SocketError socketError )
{
   if( socketError != QTcpSocket::QAbstractSocket::RemoteHostClosedError )
   {
#if SLARTCOM_DEBUG
      emit debug( QString("c:connect error %1").arg( socketError ) );
#endif
      QTimer::singleShot( 1, this, SLOT(runServer()) );
   }
#if SLARTCOM_DEBUG
   emit debug( "c:error connecting to server" );
#endif
   QTimer::singleShot( 1000, this, SLOT(start()) );
}


void SLARTCom::runServer()
{
#if SLARTCOM_DEBUG
   emit debug( "c:starting server" );
#endif
   if( mpServer == 0 )
   {
      mpServer = new SLARTComServerRunner( mPort, mHost );
      mpServer->start();
      connect( mpServer, SIGNAL(finished()),
               this, SLOT(serverShutdown()) );
#if SLARTCOM_DEBUG && SLARTComSERVER_DEBUG
      connect( mpServer, SIGNAL(debug(const QString&)),
               this,     SIGNAL(debug(const QString&)) );
#endif
   }
}


void SLARTCom::serverShutdown()
{
#if SLARTCOM_DEBUG
   emit debug( "c:stopping server (listen failed)" );
#endif
   mpServer = 0;
}


void SLARTCom::disconnected()
{
#if SLARTCOM_DEBUG
   emit debug( "c:disconnected from server" );
#endif
   QTimer::singleShot( 500, this, SLOT(start()) );
}


void SLARTCom::incomingData()
{
#if SLARTCOM_DEBUG
   QByteArray msg( mpServerConnection->readAll() );
   emit debug( QString("c:from server: ") + QString::fromUtf8(msg) );
   emit received( msg );
#else
   emit received( mpServerConnection->readAll() );
#endif
}

