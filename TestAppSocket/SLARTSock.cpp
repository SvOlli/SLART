/**
 * SLARTSock.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 */

#include "SLARTSock.hpp"

#include <QtGui>
#include <QDir>

#include "Trace.hpp"


SLARTSock::SLARTSock( QObject *parent )
: QObject( parent )
, mClientMode( false )
, mServerMode( false )
, mpServerConnection( new QLocalSocket( this ) )
, mpServer( new QLocalServer( this ) )
, mpClientsReadMapper( new QSignalMapper( this ) )
, mpClientsDisconnectMapper( new QSignalMapper( this ) )
, mClientConnections()
#ifdef Q_OS_WIN32
, mPath( "\\\\.\\PIPE\\SLART" )
#else
, mPath( QDir::home().absolutePath() + "/.SLART.sock" )
#endif
{
}


SLARTSock::~SLARTSock()
{
   deinit();
}


void SLARTSock::init()
{
   connect( mpServerConnection, SIGNAL(connected()),
            this, SLOT(serverConnectSuccess()) );
   connect( mpServerConnection, SIGNAL(disconnected()),
            this, SLOT(serverDisconnect()) );
   connect( mpServerConnection, SIGNAL(error(QLocalSocket::LocalSocketError)),
            this, SLOT(serverConnectFail(QLocalSocket::LocalSocketError)) );
   connect( mpServerConnection, SIGNAL(readyRead()),
            this, SLOT(serverHasData()) );
   connect( mpServer, SIGNAL(newConnection()),
            this, SLOT(clientConnect()) );
   connect( mpClientsReadMapper, SIGNAL(mapped(QObject*)),
            this, SLOT(clientHasData(QObject*)));
   connect( mpClientsDisconnectMapper, SIGNAL(mapped(QObject*)),
            this, SLOT(clientDisconnected(QObject*)));
   QTimer::singleShot( 1, this, SLOT(restart()) );
}


void SLARTSock::restart()
{
   if( !mServerMode && !mClientMode )
   {
      mpServerConnection->connectToServer( mPath );
   }
}


void SLARTSock::deinit()
{
   mpServer->close();
   disconnect( mpServerConnection, SIGNAL(connected()),
            this, SLOT(serverConnectSuccess()) );
   disconnect( mpServerConnection, SIGNAL(disconnected()),
            this, SLOT(restart()) );
   disconnect( mpServerConnection, SIGNAL(error(QLocalSocket::LocalSocketError)),
            this, SLOT(serverConnectFail(QLocalSocket::LocalSocketError)) );
   disconnect( mpServerConnection, SIGNAL(readyRead()),
            this, SLOT(serverHasData()) );
   disconnect( mpServer, SIGNAL(newConnection()),
            this, SLOT(clientConnect()) );
   disconnect( mpClientsReadMapper, SIGNAL(mapped(QObject*)),
            this, SLOT(clientHasData(QObject*)));
   disconnect( mpClientsDisconnectMapper, SIGNAL(mapped(QObject*)),
            this, SLOT(clientDisconnected(QObject*)));
   /* unnecessary(?) workaround for windows */
   mpServerConnection->write("client says bye");
   mpServerConnection->close();
   for( int i = 0; i < mClientConnections.count(); i++ )
   {
      QLocalSocket *socket = mClientConnections.at(i);
      disconnect(socket, SIGNAL(readyRead()),
                 mpClientsReadMapper, SLOT(map()));
      mpClientsReadMapper->removeMappings( socket );
      disconnect(socket, SIGNAL(disconnected()),
                mpClientsDisconnectMapper, SLOT(map()));
      mpClientsDisconnectMapper->removeMappings( socket );
      /* necessary workaround for windows */
      socket->write("server says bye");
      socket->close();
   }
   mClientConnections.clear();
   mClientMode = false;
   mServerMode = false;
}



void SLARTSock::send( const QString &message, QLocalSocket *omit )
{
   QByteArray msg( message.toUtf8() );
   
   if( mClientMode )
   {
      mpServerConnection->write( msg );
   }
   if( mServerMode )
   {
      for( int i = 0; i < mClientConnections.count(); i++ )
      {
         QLocalSocket *current = mClientConnections.at(i);
         if( current && (omit != current) )
         {
            current->write( msg );
         }
      }
   }
}


void SLARTSock::serverConnectSuccess()
{
   if( mServerMode )
   {
      mpServerConnection->close();
      return;
   }
   mClientMode = true;
   mServerMode = false;
   emit debug( "connected to server in client mode" );
}


void SLARTSock::serverConnectFail( QLocalSocket::LocalSocketError socketError )
{
   emit debug( "error connecting to server" );
   if( socketError != QLocalSocket::PeerClosedError )
   {
      if( mpServer->listen( mPath ) )
      {
         mClientMode = false;
         mServerMode = true;
         emit debug( "running server mode" );
         return;
      }
   }
   serverDisconnect();
}


void SLARTSock::serverDisconnect()
{
   emit debug( "disconnected from server" );
   mClientMode = false;
   mServerMode = false;
   QTimer::singleShot( 1000, this, SLOT(restart()) );
}


void SLARTSock::clientConnect()
{
   QLocalSocket *socket = mpServer->nextPendingConnection();
   connect(socket, SIGNAL(readyRead()),
           mpClientsReadMapper, SLOT(map()));
   mpClientsReadMapper->setMapping(socket, static_cast<QObject*>(socket));
   connect(socket, SIGNAL(disconnected()),
           mpClientsDisconnectMapper, SLOT(map()));
   mpClientsDisconnectMapper->setMapping(socket, static_cast<QObject*>(socket));
   mClientConnections.append( socket );
   emit debug( QString("client connected, %1 clients active")
               .arg(QString::number(mClientConnections.count())) );
}


void SLARTSock::serverHasData()
{
   QString msg( QString::fromUtf8(mpServerConnection->readAll()) );
   msg.prepend("from server: ");
   emit debug( msg );
}


void SLARTSock::clientHasData( QObject *client )
{
   QLocalSocket *socket = static_cast<QLocalSocket*>(client);
   QString msg( QString::fromUtf8(socket->readAll()) );
   send( msg, socket );
   msg.prepend("from client: ");
   emit debug( msg );
}


void SLARTSock::clientDisconnected( QObject *client )
{
   disconnect(client, SIGNAL(readyRead()),
              mpClientsReadMapper, SLOT(map()));
   mpClientsReadMapper->removeMappings( client );
   disconnect(client, SIGNAL(disconnected()),
             mpClientsDisconnectMapper, SLOT(map()));
   mpClientsDisconnectMapper->removeMappings( client );
   QLocalSocket *socket = static_cast<QLocalSocket*>(client);
   mClientConnections.removeAll( socket );
   emit debug( QString("client disconnected, %1 clients active")
               .arg(QString::number(mClientConnections.count())) );
}
