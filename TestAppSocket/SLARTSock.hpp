/**
 * SLARTSock.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 */

#ifndef SLARTSOCK_HPP
#define SLARTSOCK_HPP SLARTSOCK_HPP

#include <QObject>

#include <QString>
#include <QLocalServer>
#include <QLocalSocket>

class QIcon;
class QListWidget;
class QLineEdit;
class QSignalMapper;


class SLARTSock : public QObject
{
Q_OBJECT
   
public:
   SLARTSock( QObject *parent = 0 );
   virtual ~SLARTSock();
   void init();
   void deinit();
   
public slots:
   void send( const QString &message, QLocalSocket *omit = 0 );
   void restart();
   void serverDisconnect();
   void serverConnectSuccess();
   void serverConnectFail( QLocalSocket::LocalSocketError socketError );
   void serverHasData();
   void clientConnect();
   void clientHasData( QObject *client );
   void clientDisconnected( QObject *client );
   
signals:
   void debug( const QString &message );
   
private:
   SLARTSock( const SLARTSock &other );
   SLARTSock &operator=( const SLARTSock &other );
   
   bool                 mClientMode;
   bool                 mServerMode;
   QLocalSocket         *mpServerConnection;
   QLocalServer         *mpServer;
   QSignalMapper        *mpClientsReadMapper;
   QSignalMapper        *mpClientsDisconnectMapper;
   QList<QLocalSocket*> mClientConnections;
   QString              mPath;
};

#endif
