/**
 * src/libs/Common/SatelliteServer.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 */

#ifndef SATELLITESERVERRUNNER_HPP
#define SATELLITESERVERRUNNER_HPP SATELLITESERVERRUNNER_HPP

#include <QThread>

#include <QHostAddress>

#include "SatelliteServer.hpp"


class SatelliteServer;

class SatelliteServerRunner : public QThread
{
Q_OBJECT

public:
   SatelliteServerRunner( quint16 port, const QHostAddress &host );
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
