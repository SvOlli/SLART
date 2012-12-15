/*
 * src/libs/Common/SatelliteServer.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef SATELLITESERVER_HPP
#define SATELLITESERVER_HPP SATELLITESERVER_HPP

#ifndef SATELLITESERVER_DEBUG
#define SATELLITESERVER_DEBUG 0
#endif

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


/*!
  \addtogroup Satellite
  @{
  */

/*!
 \brief the server handling the Satellite comminication

 \dotfile "graphs/libs/Satellite/SatelliteServer_connect.dot" "Connect Graph"
*/
class SatelliteServer : public QObject
{
   Q_OBJECT

public:
   /*!
    \brief constructor

    \param parent
   */
   SatelliteServer( QObject *parent = 0 );

   /*!
    \brief destructor

   */
   virtual ~SatelliteServer();

   /*!
    \brief start the server

   */
   bool listen( const QHostAddress &host, quint16 port );

private slots:
   /*!
    \brief handle client connect

   */
   void connected();
   /*!
    \brief handle data incoming from client

    \param client
   */
   void incomingData( QObject *client );
   /*!
    \brief handle client disconnect

    \param client
   */
   void disconnected( QObject *client );

signals:
#if SATELLITESERVER_DEBUG
   /*!
    \brief output debug messages (depricated)

    \param message
   */
   void debug( const QByteArray &message );
#endif

private:
   Q_DISABLE_COPY( SatelliteServer )

   QTcpServer           *mpTcpServer; /*!< \brief the socket server */
   QSignalMapper        *mpClientsReadMapper; /*!< \brief signal mapper for handling new data */
   QSignalMapper        *mpClientsDisconnectMapper; /*!< \brief signal mapper for handling disconnects */
   QList<QTcpSocket*>   mClientConnections; /*!< \brief a list of all connections in use */
};

/*! @} */

#endif
