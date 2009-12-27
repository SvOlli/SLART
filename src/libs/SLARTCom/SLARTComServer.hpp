/**
 * src/libs/SLARTCom/SLARTComServer.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 */

#ifndef SLARTCOMSERVER_HPP
#define SLARTCOMSERVER_HPP SLARTCOMSERVER_HPP

#include <QThread>

#include <QHostAddress>
#include <QString>
#include <QTcpServer>
#include <QTcpSocket>

class QListWidget;
class QLineEdit;
class QSignalMapper;

#define SLARTCOMSERVER_DEBUG 1


class SLARTComServer : public QObject
{
Q_OBJECT
   
public:
   SLARTComServer( quint16 port, const QHostAddress &host, QObject *parent = 0 );
   virtual ~SLARTComServer();
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
#if SLARTCOMSERVER_DEBUG
   /* output debug messages (depricated) */
   void debug( const QString &message );
#endif
   
private:
   SLARTComServer( const SLARTComServer &other );
   SLARTComServer &operator=( const SLARTComServer &other );
   
   QTcpServer           *mpServer;
   QSignalMapper        *mpClientsReadMapper;
   QSignalMapper        *mpClientsDisconnectMapper;
   QList<QTcpSocket*>   mClientConnections;
   quint16              mPort;
   QHostAddress         mHost;
};
#endif
