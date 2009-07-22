/**
 * WebServer.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#ifndef WEBSERVER_HPP
#define WEBSERVER_HPP WEBSERVER_HPP

/*
 * this class is only for receiving WebServer messages 
 * sending of WebServer messages is handled by the MySettings class
 */

#include <QObject>
#include <QDir>

class QHttpRequestHeader;
class QHttpResponseHeader;
class QTcpServer;
class QTcpSocket;


class WebServer : public QObject
{
Q_OBJECT
   
public:
   WebServer( QObject *parent = 0 );
   virtual ~WebServer();
   
   bool start( quint16 port = 0, const QString &webPath = QString() );
   void stop();
   
public slots:
   void handleNewConnection();
   void response( QTcpSocket *id,
                  const QHttpResponseHeader &header,
                  const QByteArray &data );
   
signals:
   void request( QTcpSocket *id, 
                 const QHttpRequestHeader &header );
   
private:
   WebServer( const WebServer &other );
   WebServer &operator=( const WebServer &other );
   
   QObject      *mpParent;
   QTcpServer   *mpTcpServer;
   QTcpSocket   *mpTcpSocket;
   QDir         mWebDir;
};

#endif
