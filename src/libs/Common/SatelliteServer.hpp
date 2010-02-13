/**
 * src/libs/Common/SatelliteServer.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 */

#ifndef SATELLITESERVER_HPP
#define SATELLITESERVER_HPP SATELLITESERVER_HPP

#define SATELLITESERVER_DEBUG 0

#include <QThread>

#include <QHostAddress>
#include <QString>
#include <QTcpServer>
#include <QTcpSocket>

class QListWidget;
class QLineEdit;
class QSignalMapper;


class SatelliteServer : public QObject
{
Q_OBJECT
   
public:
   SatelliteServer( quint16 port, const QHostAddress &host, QObject *parent = 0 );
   virtual ~SatelliteServer();
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
#if SATELLITESERVER_DEBUG
   /* output debug messages (depricated) */
   void debug( const QByteArray &message );
#endif
   
private:
   SatelliteServer( const SatelliteServer &other );
   SatelliteServer &operator=( const SatelliteServer &other );
   
   QTcpServer           *mpServer;
   QSignalMapper        *mpClientsReadMapper;
   QSignalMapper        *mpClientsDisconnectMapper;
   QList<QTcpSocket*>   mClientConnections;
   quint16              mPort;
   QHostAddress         mHost;
};
#endif