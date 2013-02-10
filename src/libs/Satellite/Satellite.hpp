/*
 * src/libs/Satellite/Satellite.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef SATELLITE_HPP
#define SATELLITE_HPP SATELLITE_HPP

#ifndef SATELLITE_DEBUG
#define SATELLITE_DEBUG 0
#endif

/* base class */
#include <QThread>

/* system headers */

/* Qt headers */
#include <QPointer>
#include <QStringList>
#include <QtEndian>
#include <QtNetwork/QAbstractSocket>
#include <QtNetwork/QHostAddress>

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */
class QSignalMapper;
class QTcpSocket;

/* forward declaration of local classes */
class SatelliteServer;


/*!
  \addtogroup Satellite Satellite: interprocess communication

  \brief \todo
  @{
  */

/*!
 \brief central interface for Satellite interprocess communication

 \dotfile "graphs/libs/Satellite/Satellite_connect.dot" "Connect Graph"
*/
class Satellite : public QThread
{
   Q_OBJECT

public:
   /*!
    \brief create the central Satellite instance available via get()

   */
   static Satellite* create( const QHostAddress &host, quint16 port );

   /*!
    \brief destroy the central Satellite

   */
   static void destroy();

   /*!
    \brief get applications central Satellite

   */
   static Satellite* get();

   /*!
    \brief set the host and port parameters for the network connection

    \param host server host
    \param port server port
   */
   void setHostPort( const QHostAddress &host, quint16 port );

   /*!
    \brief test applications can access Satellite without enableing in settings

    \param isTestApp
   */
   void setTestApp( bool isTestApp );

   /*!
    \brief is this application running the server?

   */
   bool isRunningServer();

   /*!
    \brief send message without running a server

    (implemented in SatelliteSingleSend.cpp for linker optimizations)

    \param message
   */
   static void send1( const QByteArray &message );

   /*!
    \brief helper function: split up the lines of a Satellite message to several strings

    \param message
   */
   static QStringList split( const QByteArray &message );

public slots:
   /*!
    \brief send the message to all other clients

    \param message message to send
   */
   void send( const QByteArray &message );

private slots:
   /*!
    \brief start the client and connect to server

   */
   void restart();

   /*!
    \brief connection to server failed

    \param socketError
   */
   void connectFail( QAbstractSocket::SocketError socketError );

   /*!
    \brief server has new data from another client

   */
   void incomingData();

   /*!
    \brief connection to server was closed

   */
   void disconnected();

   /*!
    \brief try to start the server

   */
   void runServer();
#if SATELLITE_DEBUG
   /*!
    \brief send message that server is shut down

   */
   void serverShutdown();

   /*!
    \brief send message that connection to server was successful

   */
   void connectSuccess();
#endif

signals:
   /*!
    \brief connection to the bus was successful

   */
   void connected();

   /*!
    \brief the message send from another client

    \param message message received
   */
   void received( const QByteArray &message );

#if SATELLITE_DEBUG
   /*!
    \brief output debug messages (depricated)

    \param message
   */
   void debug( const QByteArray &message );
#endif

private:
   /*!
    \brief constructor

    \param parent parent object
   */
   Satellite( const QHostAddress &host, quint16 port );

   /*!
    \brief destructor

   */
   virtual ~Satellite();

   /*!
    \brief main loop

   */
   void run();

#if 0
   /*!
    \brief hook for configuration

    implemented in SatelliteConfig.cpp, which is linked in libCommon.a
    reimplement as followed in test applications:
    \code bool Satellite::enabled() { return true; } \endcode
   */
   static bool enabled();

   /*!
    \brief hook for configuration

    implemented in SatelliteConfig.cpp, which is linked in libCommon.a
   */
   static quint16 port();

   /*!
    \brief hook for configuration

    implemented in SatelliteConfig.cpp, which is linked in libCommon.a
   */
   static QHostAddress host();
#endif

   Q_DISABLE_COPY( Satellite )

   QTcpSocket                 *mpServerConnection; /*!< \brief tcp socket to server */
   QPointer<SatelliteServer>  mpServer; /*!< \brief hold server thread, if needed */
   QHostAddress               mHost; /*!< \brief server host */
   quint16                    mPort; /*!< \brief server port */

   static Satellite           *cpSatellite; /*!< \brief pointer of global Satellite */
};

#ifdef SATELLITE_PKG_HEADER
#define SATELLITE_PKGINFO_HEADER_TYPE     quint64
#define SATELLITE_PKGINFO_HEADER_SIZE     (qint64)sizeof( SATELLITE_PKGINFO_HEADER_TYPE )
#define SATELLITE_PKGINFO_MAGIC_VALUE     (quint32)0x53415400
#define SATELLITE_PKGINFO_CHECKSUM_TYPE   quint16
#define SATELLITE_PKGINFO_CHECKSUM_SIZE   (qint64)sizeof( SATELLITE_PKGINFO_CHECKSUM_TYPE )
#endif

/*! @} */

#endif
