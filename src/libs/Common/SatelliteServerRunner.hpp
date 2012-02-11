/*
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
  \addtogroup Satellite
  @{
  */

/*!
 \brief thread container to run the SatelliteServer in, started from Satellite on demand

 \dotfile "graphs/libs/Common/SatelliteServerRunner_connect.dot" "Connect Graph"
*/
class SatelliteServerRunner : public QThread
{
   Q_OBJECT

public:
   /*!
    \brief constructor

    \param host the host to bind to
    \param port the port to open
   */
   SatelliteServerRunner( const QHostAddress &host, quint16 port );

   /*!
    \brief destructor

   */
   virtual ~SatelliteServerRunner();

   /*!
    \brief method called by QThread::start() to to run the server

   */
   void run();

signals:
#if SATELLITESERVER_DEBUG
   /*!
    \brief output debug messages (depricated)

    \param message
   */
   void debug( const QByteArray &message );
#endif

private:
   Q_DISABLE_COPY( SatelliteServerRunner )

   SatelliteServer      *mpServer; /*!< \brief the server running */
   quint16              mPort; /*!< \brief the port to open */
   QHostAddress         mHostAddress; /*! \brief the host to bind to */
};

/*! @} */

#endif
