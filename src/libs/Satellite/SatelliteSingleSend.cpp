/*
 * src/libs/Common/SatelliteSingleSend.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

/* class declaration */
#include "SatelliteSingleSend.hpp"

/* system headers */

/* Qt headers */
#include <QCoreApplication>
#include <QHostAddress>
#include <QTcpSocket>

/* local library headers */

/* local headers */
#define SATELLITE_PKG_HEADER SATELLITE_PKG_HEADER
#include "Satellite.hpp"

/* class variable instantiation */


QObject *SatelliteSingleSend::send( const QHostAddress &host, quint16 port,
                                    const QByteArray &message )
{
   return new SatelliteSingleSend( host, port, message );
}


SatelliteSingleSend::SatelliteSingleSend( const QHostAddress &host, quint16 port,
                                          const QByteArray &message )
: QObject( QCoreApplication::instance() )
, mpServerConnection( new QTcpSocket( this ) )
, mMessage( message )
{
   connect( mpServerConnection, SIGNAL(connected()),
            this, SLOT(connected()) );
   connect( mpServerConnection, SIGNAL(disconnected()),
            this, SLOT(deleteLater()) );
   connect( mpServerConnection, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(deleteLater()) );

   mpServerConnection->connectToHost( host, port, QIODevice::WriteOnly | QIODevice::Unbuffered );
}


SatelliteSingleSend::~SatelliteSingleSend()
{
   emit done();
}


void SatelliteSingleSend::connected()
{
   SATELLITE_PKGINFO_HEADER_TYPE   header( SATELLITE_PKGINFO_MAGIC_VALUE );
   header <<= 32;
   header |= mMessage.size();
   header = qToBigEndian( header );
   SATELLITE_PKGINFO_CHECKSUM_TYPE checksum( qChecksum( mMessage.constData(), mMessage.size() ) );
   checksum = qToBigEndian( checksum );
   mpServerConnection->write( (char*)(&header), SATELLITE_PKGINFO_HEADER_SIZE );
   mpServerConnection->write( mMessage );
   mpServerConnection->write( (char*)(&checksum), SATELLITE_PKGINFO_CHECKSUM_SIZE );
   mpServerConnection->flush();
   mpServerConnection->disconnectFromHost();
}
