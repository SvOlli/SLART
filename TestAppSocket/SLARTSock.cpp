/**
 * SLARTSock.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 */

#include "SLARTSock.hpp"
#include "SLARTSockServer.hpp"

#include <QtGui>
#include <QDir>

#include "Trace.hpp"

#ifdef Q_OS_MACX
#include <errno.h>
#include <string.h>
#endif

SLARTSock::SLARTSock( QObject *parent )
: QObject( parent )
#ifdef Q_OS_WIN32
, mPath( "\\\\.\\PIPE\\SLART" )
#else
, mPath( QDir::home().absolutePath() + "/.SLART.sock" )
#endif
, mpServerConnection( new QLocalSocket( this ) )
, mpServer( 0 )
{
   connect( mpServerConnection, SIGNAL(error(QLocalSocket::LocalSocketError)),
            this, SLOT(connectFail(QLocalSocket::LocalSocketError)) );
#if SLARTSOCK_DEBUG
   connect( mpServerConnection, SIGNAL(connected()),
            this, SLOT(connectSuccess()) );
#endif
   connect( mpServerConnection, SIGNAL(disconnected()),
            this, SLOT(disconnected()) );
   connect( mpServerConnection, SIGNAL(readyRead()),
            this, SLOT(incomingData()) );
}


SLARTSock::~SLARTSock()
{
}


void SLARTSock::start()
{
#if SLARTSOCK_DEBUG
   emit debug( "c:connecting to server" );
#endif
   mpServerConnection->connectToServer( mPath );
}


void SLARTSock::send( const QString &message )
{
   mpServerConnection->write( message.toUtf8() );
}


#if SLARTSOCK_DEBUG
void SLARTSock::connectSuccess()
{
   emit debug( "c:connected to server in client mode" );
}
#endif


void SLARTSock::connectFail( QLocalSocket::LocalSocketError socketError )
{
   Q_UNUSED( socketError );
   if( socketError != QLocalSocket::PeerClosedError )
   {
#if SLARTSOCK_DEBUG
      emit debug( QString("c:connect error %1").arg( socketError ) );
      emit debug( "c:starting server" );
#endif
      if( mpServer == 0 )
      {
         mpServer = new SLARTSockServer( mPath, this );
#if SLARTSOCK_DEBUG && SLARTSOCKSERVER_DEBUG
         connect( mpServer, SIGNAL(debug(const QString&)),
                  this,     SIGNAL(debug(const QString&)) );
#endif
         if( !mpServer->listen() )
         {
#if SLARTSOCK_DEBUG
            emit debug( "c:stopping server (listen failed)" );
#endif
            delete mpServer;
            mpServer = 0;
         }
      }
   }
#if SLARTSOCK_DEBUG
   emit debug( "c:error connecting to server" );
#endif
   QTimer::singleShot( 1000, this, SLOT(start()) );
}


void SLARTSock::disconnected()
{
#if SLARTSOCK_DEBUG
   emit debug( "c:disconnected from server" );
#endif
   QTimer::singleShot( 500, this, SLOT(start()) );
}


void SLARTSock::incomingData()
{
   QString msg( QString::fromUtf8(mpServerConnection->readAll()) );
#if SLARTSOCK_DEBUG
   emit debug( QString("c:from server: ") + msg );
#endif
   emit received( msg );
}

