/**
 * src/libs/Common/SatelliteServerRunner.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 */

#include "SatelliteServerRunner.hpp"

#include "SatelliteServer.hpp"


SatelliteServerRunner::SatelliteServerRunner( quint16 port, const QHostAddress &host )
: QThread()
, mPort( port )
, mHost( host )
{
}


void SatelliteServerRunner::run()
{
   SatelliteServer *mpServer = new SatelliteServer( mPort, mHost, 0 );
#if SATELLITESERVER_DEBUG
   connect( mpServer, SIGNAL(debug(const QString&)),
            this,     SIGNAL(debug(const QString&)) );
#endif
   if( mpServer->listen() )
   {
      exec();
   }
   mpServer->deleteLater();
   this->deleteLater();
}
