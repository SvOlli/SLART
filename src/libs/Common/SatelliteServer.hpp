/**
 * src/libs/Common/SatelliteServer.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef SATELLITESERVER_HPP
#define SATELLITESERVER_HPP SATELLITESERVER_HPP

#define SATELLITESERVER_DEBUG 0

/* base class */
#include <QObject>

/* system headers */

/* Qt headers */
#include <QHostAddress>

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */
class QLineEdit;
class QListWidget;
class QSignalMapper;
class QTcpServer;
class QTcpSocket;

/* forward declaration of local classes */


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
   SatelliteServer( const SatelliteServer &that );
   SatelliteServer &operator=( const SatelliteServer &that );
   
   QTcpServer           *mpTcpServer;
   QSignalMapper        *mpClientsReadMapper;
   QSignalMapper        *mpClientsDisconnectMapper;
   QList<QTcpSocket*>   mClientConnections;
   quint16              mPort;
   QHostAddress         mHost;
};
#endif
