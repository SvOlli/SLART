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

class SLARTSockServer;

#define SLARTSOCK_DEBUG 1

class SLARTSock : public QObject
{
Q_OBJECT
   
public:
   SLARTSock( QObject *parent = 0 );
   virtual ~SLARTSock();
   
public slots:
   /* start the client and connect to server */
   void start();
   /* send the message to all other clients */
   void send( const QString &message );
   
private slots:
#if SLARTSOCK_DEBUG
   /* connection to server was successful */
   void connectSuccess();
#endif
   /* connection to server failed */
   void connectFail( QLocalSocket::LocalSocketError socketError );
   /* server has new data from another client */
   void incomingData();
   /* connection to server was closed */
   void disconnected();
   
signals:
   /* the message send from another client */
   void received( const QString &message );
#if SLARTSOCK_DEBUG
   /* output debug messages (depricated) */
   void debug( const QString &message );
#endif
   
private:
   SLARTSock( const SLARTSock &other );
   SLARTSock &operator=( const SLARTSock &other );
   
   QString              mPath;
   QLocalSocket         *mpServerConnection;
   SLARTSockServer      *mpServer;
};

#endif
