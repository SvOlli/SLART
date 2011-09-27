/**
 * src/libs/Common/SatelliteServerRunner.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

/* class declaration */
#include "SatelliteServerRunner.hpp"

/* system headers */

/* Qt headers */

/* local library headers */

/* local headers */
#include "SatelliteServer.hpp"


SatelliteServerRunner::SatelliteServerRunner( const QHostAddress &host, quint16 port )
: QThread()
, mpServer( 0 )
, mPort( port )
, mHostAddress( host )
{
}


SatelliteServerRunner::~SatelliteServerRunner()
{
   if( isRunning() )
   {
      quit();
      wait();
   }
}


void SatelliteServerRunner::run()
{
   SatelliteServer *mpServer = new SatelliteServer();
#if SATELLITESERVER_DEBUG
   connect( mpServer, SIGNAL(debug(QByteArray)),
            this,     SIGNAL(debug(QByteArray)) );
#endif
   if( mpServer->listen( mHostAddress, mPort ) )
   {
      exec();
   }
   mpServer->deleteLater();
   mpServer = 0;
   this->deleteLater();
}
