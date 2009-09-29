/**
 * SLARTSockServer.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 */

#ifndef SLARTSOCKSERVER_HPP
#define SLARTSOCKSERVER_HPP SLARTSOCKSERVER_HPP

#include <QObject>

#include <QString>
#include <QLocalServer>
#include <QLocalSocket>
#include <QSharedMemory>

class QIcon;
class QListWidget;
class QLineEdit;
class QSignalMapper;

#define SLARTSOCKSERVER_DEBUG 1

class SLARTSockServer : public QObject
{
Q_OBJECT
   
public:
   SLARTSockServer( const QString &path, QObject *parent = 0 );
   virtual ~SLARTSockServer();
   /* start the server */
   bool listen();
   
private slots:
   /* handle client connect */
   void connected();
   /* handle data incoming from client */
   void incomingData( QObject *client );
   /* handle client disconnect */
   void disconnected( QObject *client );
   
signals:
#if SLARTSOCKSERVER_DEBUG
   /* output debug messages (depricated) */
   void debug( const QString &message );
#endif
   
private:
   SLARTSockServer( const SLARTSockServer &other );
   SLARTSockServer &operator=( const SLARTSockServer &other );
   
   QLocalServer         *mpServer;
   QSignalMapper        *mpClientsReadMapper;
   QSignalMapper        *mpClientsDisconnectMapper;
   QList<QLocalSocket*> mClientConnections;
   QSharedMemory        mShMem;
   QString              mPath;
};

#endif
