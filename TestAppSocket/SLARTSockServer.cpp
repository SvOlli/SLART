/**
 * SLARTSockServer.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 */

#include "SLARTSockServer.hpp"

#include <QtGui>
#include <QDir>

#ifndef Q_OS_WIN32
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#endif

#include "Trace.hpp"


SLARTSockServer::SLARTSockServer( const QString &path, QObject *parent )
: QObject( parent )
, mpServer( new QLocalServer( this ) )
, mpClientsReadMapper( new QSignalMapper( this ) )
, mpClientsDisconnectMapper( new QSignalMapper( this ) )
, mClientConnections()
, mShMem( path )
, mPath( path )
{
   connect( mpServer, SIGNAL(newConnection()),
            this, SLOT(connected()) );
   connect( mpClientsReadMapper, SIGNAL(mapped(QObject*)),
            this, SLOT(incomingData(QObject*)));
   connect( mpClientsDisconnectMapper, SIGNAL(mapped(QObject*)),
            this, SLOT(disconnected(QObject*)));
}


SLARTSockServer::~SLARTSockServer()
{
   mpServer->close();
   mShMem.detach();
   for( int i = 0; i < mClientConnections.count(); i++ )
   {
      QLocalSocket *socket = mClientConnections.at(i);
      /* write is workaround for windows */
      socket->write("server says bye");
      socket->close();
   }
}


bool SLARTSockServer::listen()
{
#ifdef SLARTSOCKSERVER_DEBUG
   emit debug( QString( "running server on "+mPath ) );
#endif
   /* the whole magic to creating only one server happens here:
      using a shared memory segment as semaphore */
   if( !mShMem.create( 242 ) )
   {
#ifdef SLARTSOCKSERVER_DEBUG
      emit debug( QString("shmem create failed %1: %2 (%3)")
                     .arg( mShMem.key(), mShMem.errorString() )
                     .arg( QString::number(mShMem.error()) ) );
#endif
      
      return false;
   }
   
   bool success = mpServer->listen( mPath );
   
#if 0   
#ifdef Q_OS_MACX
   /* workaround for mac os x: remove stale socket */
   if( !success )
   {
      emit debug("removing stale socket");
      QFile::remove( mPath );
      success = mpServer->listen( mPath );
   }
#endif
#endif
   
#ifndef Q_OS_WIN32
   /* workaround for not deleting shm-segment on crash:
      remove it immediantly after creation */
   QDir tmpDir(QDir::temp());
   
   QString base( mPath );
   base.replace( QRegExp(QLatin1String("[^A-Za-z]")), QString() );
   base.prepend( "qipc_sharedmemory_" );
   base.append("*");
   
   QStringList matchList(tmpDir.entryList(QStringList() << base));
   
   if( matchList.count() > 0 )
   {
      key_t key = ftok( QFile::encodeName(tmpDir.absoluteFilePath(matchList.at(0))).constData(), 'Q' );
      int shmid = shmget( key, 0, 0660 );
      if( !shmctl(shmid, IPC_RMID, NULL) )
      {
         /* on success remove file as well */
         QFile::remove( tmpDir.absoluteFilePath(matchList.at(0)) );
      }
   }
#endif
   
   return success;
}


void SLARTSockServer::incomingData( QObject *client )
{
   QLocalSocket *socket = static_cast<QLocalSocket*>(client);
   QByteArray msg( socket->readAll() );
   
   for( int i = 0; i < mClientConnections.count(); i++ )
   {
      QLocalSocket *current = mClientConnections.at(i);
      if( current && (client != current) )
      {
         current->write( msg );
      }
   }
#ifdef SLARTSOCKSERVER_DEBUG
   msg.prepend("from client: ");
   emit debug( msg );
#endif
}


void SLARTSockServer::connected()
{
   QLocalSocket *socket = mpServer->nextPendingConnection();
   connect(socket, SIGNAL(readyRead()),
           mpClientsReadMapper, SLOT(map()));
   mpClientsReadMapper->setMapping(socket, static_cast<QObject*>(socket));
   connect(socket, SIGNAL(disconnected()),
           mpClientsDisconnectMapper, SLOT(map()));
   mpClientsDisconnectMapper->setMapping(socket, static_cast<QObject*>(socket));
   mClientConnections.append( socket );
#ifdef SLARTSOCKSERVER_DEBUG
   emit debug( QString("client connected, %2 clients active")
               .arg(QString::number(mClientConnections.count())) );
#endif
}


void SLARTSockServer::disconnected( QObject *client )
{
   disconnect(client, SIGNAL(readyRead()),
              mpClientsReadMapper, SLOT(map()));
   mpClientsReadMapper->removeMappings( client );
   disconnect(client, SIGNAL(disconnected()),
             mpClientsDisconnectMapper, SLOT(map()));
   mpClientsDisconnectMapper->removeMappings( client );
   QLocalSocket *socket = static_cast<QLocalSocket*>(client);
   mClientConnections.removeAll( socket );
#ifdef SLARTSOCKSERVER_DEBUG
   emit debug( QString("client disconnected, %1 clients active")
               .arg(QString::number(mClientConnections.count())) );
#endif
}
