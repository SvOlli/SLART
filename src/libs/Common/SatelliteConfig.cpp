/*
 * src/libs/Common/SatelliteConfig.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

/* class declaration */
#include "Satellite.hpp"

/* system headers */

/* Qt headers */

/* local library headers */

/* local headers */
#include "Settings.hpp"


bool Satellite::enabled()
{
   return Settings::value( Settings::CommonUseSatellite );
}


quint16 Satellite::port()
{
   return Settings::value( Settings::GlobalSatellitePort );
}


QHostAddress Satellite::host()
{
   return QHostAddress( Settings::value( Settings::GlobalSatelliteHost ) );
}
