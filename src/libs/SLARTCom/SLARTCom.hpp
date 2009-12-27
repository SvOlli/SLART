/**
 * src/libs/SLARTCom/SLARTCom.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 */

#ifndef SLARTCOM_HPP
#define SLARTCOM_HPP SLARTCOM_HPP

#include <QObject>

#include <QHostAddress>
#include <QString>
#include <QTcpSocket>

class QIcon;
class QListWidget;
class QLineEdit;
class QSignalMapper;

class SLARTComServer;
class SLARTComServerRunner;

#define SLARTCOM_DEBUG 0


class SLARTCom : public QObject
{
Q_OBJECT
   
public:
   SLARTCom( quint16 port, QObject *parent = 0 );
   virtual ~SLARTCom();
   /*  */
   void setHost( const QHostAddress &host );
   /*  */
   bool isRunningServer();
   
public slots:
   /* start the client and connect to server */
   void start();
   /* send the message to all other clients */
   void send( const QByteArray &message );
   
private slots:
   /* connection to server failed */
   void connectFail( QAbstractSocket::SocketError socketError );
   /* server has new data from another client */
   void incomingData();
   /* connection to server was closed */
   void disconnected();
   /*  */
   void runServer();
   /*  */
   void serverShutdown();
#if SLARTCOM_DEBUG
   /* connection to server was successful */
   void connectSuccess();
#endif
   
signals:
   /* the message send from another client */
   void received( const QByteArray &message );
#if SLARTCOM_DEBUG
   /* output debug messages (depricated) */
   void debug( const QString &message );
#endif
   
private:
   SLARTCom( const SLARTCom &other );
   SLARTCom &operator=( const SLARTCom &other );
   
   QTcpSocket           *mpServerConnection;
   SLARTComServerRunner *mpServer;
   quint16              mPort;
   QHostAddress         mHost;
};

#endif
