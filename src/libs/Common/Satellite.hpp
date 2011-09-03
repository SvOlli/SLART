/**
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
#include <QHostAddress>
#include <QPointer>
#include <QString>
#include <QTcpSocket>
#include <QtEndian>

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */
class QIcon;
class QListWidget;
class QLineEdit;
class QSignalMapper;

/* forward declaration of local classes */
class SatelliteServer;
class SatelliteServerRunner;


/*!
 \brief central interface for Satellite interprocess communication

 \class Satellite Satellite.hpp "libs/Common/Satellite.hpp"
*/
class Satellite : public QObject
{
Q_OBJECT

public:
   /*!
    \brief create the central Satellite instance available via get()

    \fn create
   */
   static void create();
   /*!
    \brief destroy the central Satellite

    \fn destroy
   */
   static void destroy();
   /*!
    \brief get applications central Satellite

    \fn get
   */
   static Satellite* get();

   /*!
    \brief test applications can access Satellite without enableing in settings

    \fn setTestApp
    \param isTestApp
   */
   void setTestApp( bool isTestApp );
   /*!
    \brief is this application running the server?

    \fn isRunningServer
   */
   bool isRunningServer();
   /*!
    \brief wait for a connection, so application can write to satellite right away

    \fn waitForConnected
    \param msecs maximum time to wait
      */
   bool waitForConnected( int msecs = 30000 );

   /*!
    \brief send message without running a server

    \fn send1
    \param message
   */
   static void send1( const QByteArray &message );
   /*!
    \brief helper function: split up the lines of a Satellite message to several strings

    \fn split
    \param message
   */
   static QStringList split( const QByteArray &message );

public slots:
   /*!
    \brief start the client and connect to server

    \fn restart
   */
   void restart();
   /*!
    \brief send the message to all other clients

    \fn send
    \param message
   */
   void send( const QByteArray &message );

private slots:
   /*!
    \brief connection to server failed

    \fn connectFail
    \param socketError
   */
   void connectFail( QAbstractSocket::SocketError socketError );
   /*!
    \brief server has new data from another client

    \fn incomingData
   */
   void incomingData();
   /*!
    \brief connection to server was closed

    \fn disconnected
   */
   void disconnected();
   /*!
    \brief try to start the server

    \fn runServer
   */
   void runServer();
#if SATELLITE_DEBUG
   /*!
    \brief send message that server is shut down

    \fn serverShutdown
   */
   void serverShutdown();
   /*!
    \brief send message that connection to server was successful

    \fn connectSuccess
   */
   void connectSuccess();
#endif

signals:
   /*!
    \brief connection to the bus was successful

    \fn connected
   */
   void connected();
   /*!
    \brief the message send from another client

    \fn received
    \param message
   */
   void received( const QByteArray &message );
#if SATELLITE_DEBUG
   /*!
    \brief output debug messages (depricated)

    \fn debug
    \param message
   */
   void debug( const QByteArray &message );
#endif

private:
   Satellite( QObject *parent = 0 );
   virtual ~Satellite();

   Satellite( const Satellite &that );
   Satellite &operator=( const Satellite &that );

   bool                             mIsTestApp;
   QTcpSocket                       *mpServerConnection;
   QPointer<SatelliteServerRunner>  mpServer;
   quint16                          mPort;
   QHostAddress                     mHost;

   static QPointer<Satellite>       cpSatellite;
};

#define SATELLITE_PKGINFO_HEADER_TYPE     quint64
#define SATELLITE_PKGINFO_HEADER_SIZE     (qint64)sizeof( SATELLITE_PKGINFO_HEADER_TYPE )
#define SATELLITE_PKGINFO_MAGIC_VALUE     (quint32)0x53415400
#define SATELLITE_PKGINFO_CHECKSUM_TYPE   quint16
#define SATELLITE_PKGINFO_CHECKSUM_SIZE   (qint64)sizeof( SATELLITE_PKGINFO_CHECKSUM_TYPE )

#define VALUE_SATELLITE_HOST  value( "SatelliteHost", "127.0.0.1" ).toString()
#define VALUE_SATELLITE_PORT  value( "SatellitePort", 24222 ).toInt()
#define VALUE_USE_SATELLITE   value( "UseSatellite", false ).toBool()

#endif
