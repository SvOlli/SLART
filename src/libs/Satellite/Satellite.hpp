/*
 * src/libs/Satellite/Satellite.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef SATELLITE_HPP
#define SATELLITE_HPP SATELLITE_HPP

#define SATELLITE_DEBUG 0

/* base class */
#include <QObject>

/* system headers */

/* Qt headers */
#include <QPointer>
#include <QString>
#include <QtEndian>
#include <QtNetwork/QAbstractSocket>

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */
class QHostAddress;
class QListWidget;
class QLineEdit;
class QSignalMapper;
class QTcpSocket;

/* forward declaration of local classes */
class SatelliteServer;
class SatelliteServerRunner;


/*!
  \addtogroup Satellite Satellite: interprocess communication

  \brief part of \ref Common
  @{
  */

/*!
 \brief central interface for Satellite interprocess communication

 \dotfile "graphs/libs/Common/Satellite_connect.dot" "Connect Graph"
*/
class Satellite : public QObject
{
   Q_OBJECT

public:
   /*!
    \brief create the central Satellite instance available via get()

   */
   static Satellite* create();

   /*!
    \brief destroy the central Satellite

   */
   static void destroy();

   /*!
    \brief get applications central Satellite

   */
   static Satellite* get();

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
    \brief wait for a connection, so application can write to satellite right away

    \param msecs maximum time to wait
      */
   bool waitForConnected( int msecs = 30000 );

   /*!
    \brief send message without running a server

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
    \brief start the client and connect to server

   */
   void restart();

   /*!
    \brief send the message to all other clients

    \param message message to send
   */
   void send( const QByteArray &message );

private slots:
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
   Satellite( QObject *parent = 0 );

   /*!
    \brief destructor

   */
   virtual ~Satellite();

   /*!
    \brief hook for configuration

    implemented in SatelliteConfig.cpp
   */
   static bool enabled();

   /*!
    \brief hook for configuration

    implemented in SatelliteConfig.cpp
   */
   static quint16 port();

   /*!
    \brief hook for configuration

    implemented in SatelliteConfig.cpp
   */
   static QHostAddress host();

   Q_DISABLE_COPY( Satellite )

   bool                             mIsTestApp; /*!< \brief a test app always uses Satellite */
   QTcpSocket                       *mpServerConnection; /*!< \brief tcp socket to server */
   QPointer<SatelliteServerRunner>  mpServer; /*!< \brief hold server thread, if needed */

   static Satellite                 *cpSatellite; /*!< \brief pointer of global Satellite */
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
