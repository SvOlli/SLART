/**
 * src/libs/Common/SatelliteServerRunner.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 */

/* class declaration */
#include "SatelliteServerRunner.hpp"

/* system headers */

/* Qt headers */

/* local library headers */

/* local headers */
#include "SatelliteServer.hpp"


SatelliteServerRunner::SatelliteServerRunner( quint16 port, const QHostAddress &host )
: QThread()
, mpServer( 0 )
, mPort( port )
, mHost( host )
{
}


SatelliteServerRunner::~SatelliteServerRunner()
{
   if( mpServer )
   {
      mpServer->deleteLater();
   }
}


void SatelliteServerRunner::run()
{
   SatelliteServer *mpServer = new SatelliteServer( mPort, mHost, 0 );
#if SATELLITESERVER_DEBUG
   connect( mpServer, SIGNAL(debug(const QByteArray&)),
            this,     SIGNAL(debug(const QByteArray&)) );
#endif
   if( mpServer->listen() )
   {
      exec();
   }
   mpServer->deleteLater();
   mpServer = 0;
   this->deleteLater();
}
