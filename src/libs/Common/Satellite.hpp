/**
 * src/libs/Satellite/Satellite.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 */

#ifndef SATELLITE_HPP
#define SATELLITE_HPP SATELLITE_HPP

#define SATELLITE_DEBUG 0

#include <QObject>

#include <QHostAddress>
#include <QPointer>
#include <QString>
#include <QTcpSocket>

class QIcon;
class QListWidget;
class QLineEdit;
class QSignalMapper;

class SatelliteServer;
class SatelliteServerRunner;



class Satellite : public QObject
{
Q_OBJECT
   
public:
   Satellite( QObject *parent = 0 );
   virtual ~Satellite();
   /*  */
   bool isRunningServer();

   /* get applications central Satellite */
   static Satellite* get( QObject *parent = 0 );
   /* destructor for the central Satellite */
   static void destroy();
   /* send message without running a server */
   static void send1( const QByteArray &message );
   /*  */
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
   /*  */
   void runServer();
   /*  */
   void serverShutdown();
#if SATELLITE_DEBUG
   /* connection to server was successful */
   void connectSuccess();
#endif
   
signals:
   /* the message send from another client */
   void received( const QByteArray &message );
#if SATELLITE_DEBUG
   /* output debug messages (depricated) */
   void debug( const QByteArray &message );
#endif
   
private:
   Satellite( const Satellite &other );
   Satellite &operator=( const Satellite &other );
   
   QTcpSocket                 *mpServerConnection;
   SatelliteServerRunner      *mpServer;
   quint16                    mPort;
   QHostAddress               mHost;

   static QPointer<Satellite> gSatellite;
};

#define SATELLITE_HEADER_TYPE unsigned int
#define SATELLITE_HEADER_SIZE (qint64)sizeof( SATELLITE_HEADER_TYPE )

#define VALUE_SATELLITE_HOST   value( "SatelliteHost", "127.0.0.1" ).toString()
#define VALUE_SATELLITE_PORT   value( "SatellitePort", 24222 ).toInt()
#define VALUE_USE_SATELLITE    value( "UseSatellite", false ).toBool()

#endif
