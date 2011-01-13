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


class Satellite : public QObject
{
Q_OBJECT

public:
   /*  */
   static void create();
   /* destructor for the central Satellite */
   static void destroy();
   /* get applications central Satellite */
   static Satellite* get();

   /* test applications can access Satellite without enableing in settings */
   void setTestApp( bool isTestApp );
   /* is this application running the server? */
   bool isRunningServer();
   /* wait for a connection, so application can write to satellite right away */
   bool waitForConnected( int msecs = 30000 );

   /* send message without running a server */
   static void send1( const QByteArray &message );
   /* split up the lines of a Satellite message to several strings */
   static QStringList split( const QByteArray &message );

public slots:
   /* start the client and connect to server */
   void restart();
   /* send the message to all other clients */
   void send( const QByteArray &message );

private slots:
   /* connection to server failed */
   void connectFail( QAbstractSocket::SocketError socketError );
   /* server has new data from another client */
   void incomingData();
   /* connection to server was closed */
   void disconnected();
   /* try to start the server */
   void runServer();
#if SATELLITE_DEBUG
   /* send message that server is shut down */
   void serverShutdown();
   /* send message that connection to server was successful */
   void connectSuccess();
#endif

signals:
   /* connection to the bus was successful */
   void connected();
   /* the message send from another client */
   void received( const QByteArray &message );
#if SATELLITE_DEBUG
   /* output debug messages (depricated) */
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
