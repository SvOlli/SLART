/*
 * src/libs/SingleSend/SatelliteSingleSend.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef SATELLITESINGLESEND_HPP
#define SATELLITESINGLESEND_HPP SATELLITESINGLESEND_HPP

/* base class */
#include <QObject>

/* system headers */

/* Qt headers */
#include <QByteArray>
#include <QtEndian>
#include <QtNetwork/QAbstractSocket>

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */
class QTcpSocket;

/* forward declaration of local classes */


/*!
  \addtogroup Satellite

  @{
  */

/*!
 \brief helper class to send a single message via Satellite without running
 an instance

 \dotfile "graphs/libs/Satellite/SatelliteSingleSend_connect.dot" "Connect Graph"
*/
class SatelliteSingleSend : public QObject
{
   Q_OBJECT

public:
   /*!
    \brief send a single message via Satellite (only if server exists)

    \param host host to connect to
    \param port port to connect to
    \param message message to send
    \return object that holds message, which will emit signal done on completion
   */
   static QObject *send( const QHostAddress &host, quint16 port, const QByteArray &message );

private slots:
   /*!
    \brief handle connection: send out stored message

   */
   void connected();

signals:
   /*!
    \brief emitted upon deletion to show job is done

   */
   void done();

private:
   /*!
    \brief constructor

    \param host host to connect to (usually set from Satellite)
    \param port port to connect to (usually set from Satellite)
    \param message message to send
   */
   SatelliteSingleSend( const QHostAddress &host, quint16 port, const QByteArray &message );

   /*!
    \brief destructor

   */
   virtual ~SatelliteSingleSend();

   Q_DISABLE_COPY( SatelliteSingleSend )

   QTcpSocket        *mpServerConnection; /*!< \brief tcp socket to server */
   const QByteArray  mMessage; /*!< \brief message to send */
};

#ifdef SATELLITE_PKG_HEADER
#define SATELLITE_PKGINFO_HEADER_TYPE     quint64
#define SATELLITE_PKGINFO_HEADER_SIZE     (qint64)sizeof( SATELLITE_PKGINFO_HEADER_TYPE )
#define SATELLITE_PKGINFO_MAGIC_VALUE     (quint32)0x53415400
#define SATELLITE_PKGINFO_CHECKSUM_TYPE   quint16
#define SATELLITE_PKGINFO_CHECKSUM_SIZE   (qint64)sizeof( SATELLITE_PKGINFO_CHECKSUM_TYPE )
#endif

/*! @} */

#endif
