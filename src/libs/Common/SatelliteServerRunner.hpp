/**
 * src/libs/Common/SatelliteServer.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 */

#ifndef SATELLITESERVERRUNNER_HPP
#define SATELLITESERVERRUNNER_HPP SATELLITESERVERRUNNER_HPP

/* base class */
#include <QThread>

/* system headers */

/* Qt headers */
#include <QHostAddress>

/* local library headers */

/* local headers */
#include "SatelliteServer.hpp"

/* forward declaration of Qt classes */

/* forward declaration of local classes */
class SatelliteServer;


class SatelliteServerRunner : public QThread
{
Q_OBJECT

public:
   SatelliteServerRunner( quint16 port, const QHostAddress &host );
   virtual ~SatelliteServerRunner();
   /*  */
   void run();

signals:
#if SATELLITESERVER_DEBUG
   /* output debug messages (depricated) */
   void debug( const QByteArray &message );
#endif

private:
   SatelliteServer      *mpServer;
   const quint16        mPort;
   const QHostAddress   mHost;
};
#endif
