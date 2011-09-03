/**
 * src/libs/Common/SatelliteServer.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
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


/*!
 \brief thread container to run the Satellite server in, started from Satellite on demand

 \class SatelliteServerRunner SatelliteServerRunner.hpp "libs/Common/SatelliteServerRunner.hpp"
*/
class SatelliteServerRunner : public QThread
{
Q_OBJECT

public:
   SatelliteServerRunner( quint16 port, const QHostAddress &host );
   virtual ~SatelliteServerRunner();
   /*!
    \brief method called by QThread::start() to to run the server

    \fn run
   */
   void run();

signals:
#if SATELLITESERVER_DEBUG
   /*!
    \brief output debug messages (depricated)

    \fn debug
    \param message
   */
   void debug( const QByteArray &message );
#endif

private:
   SatelliteServerRunner( const SatelliteServerRunner &that );
   SatelliteServerRunner &operator=( const SatelliteServerRunner &that );

   SatelliteServer      *mpServer;
   const quint16        mPort;
   const QHostAddress   mHost;
};
#endif
