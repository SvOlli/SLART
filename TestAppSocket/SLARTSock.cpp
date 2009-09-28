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
   connect( mpServerConnection, SIGNAL(connected()),
            this, SLOT(connectSuccess()) );
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
#ifdef SLARTSOCK_DEBUG
   emit debug( "connecting to server" );
#endif
   mpServerConnection->connectToServer( mPath );
}


void SLARTSock::send( const QString &message )
{
   mpServerConnection->write( message.toUtf8() );
}


void SLARTSock::connectSuccess()
{
#ifdef SLARTSOCK_DEBUG
   emit debug( "connected to server in client mode" );
#endif
}


void SLARTSock::connectFail( QLocalSocket::LocalSocketError socketError )
{
   Q_UNUSED( socketError );
   if( socketError != QLocalSocket::PeerClosedError )
   {
#ifdef SLARTSOCK_DEBUG
      emit debug( QString("connect error %1").arg( socketError ) );
#ifdef Q_OS_MACX
      emit debug( strerror( errno ) );
#endif
      emit debug( "starting server" );
#endif
      if( mpServer == 0 )
      {
         mpServer = new SLARTSockServer( mPath, this );
#ifdef SLARTSOCKSERVER_DEBUG
         connect( mpServer, SIGNAL(debug(const QString&)),
                  this,     SIGNAL(debug(const QString&)) );
#endif
         if( !mpServer->listen() )
         {
#ifdef SLARTSOCK_DEBUG
            emit debug( "stopping server (listen failed)" );
#endif
            delete mpServer;
            mpServer = 0;
         }
      }
   }
#ifdef SLARTSOCK_DEBUG
   emit debug( "error connecting to server" );
#endif
   QTimer::singleShot( 1000, this, SLOT(start()) );
}


void SLARTSock::disconnected()
{
#ifdef SLARTSOCK_DEBUG
   emit debug( "disconnected from server" );
#endif
   QTimer::singleShot( 1, this, SLOT(start()) );
}


void SLARTSock::incomingData()
{
   QString msg( QString::fromUtf8(mpServerConnection->readAll()) );
   emit received( msg );
#ifdef SLARTSOCK_DEBUG
   msg.prepend("from server: ");
   emit debug( msg );
#endif
}

