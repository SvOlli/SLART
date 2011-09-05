/**
 * src/libs/Common/WebServer.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef WEBSERVER_HPP
#define WEBSERVER_HPP WEBSERVER_HPP

/* base class */
#include <QObject>

/* system headers */

/* Qt headers */
#include <QDir>

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */
class QHttpRequestHeader;
class QHttpResponseHeader;
class QTcpServer;
class QTcpSocket;


/*!
  \addtogroup Common

  @{
*/

/*!
 \brief TODO

*/
class WebServer : public QObject
{
   Q_OBJECT

public:
   WebServer( QObject *parent = 0 );
   virtual ~WebServer();

   bool start( quint16 port = 0, const QString &webPath = QString() );
   void stop();

public slots:
   void handleNewConnection();
   void response( QTcpSocket *id,
                  const QHttpResponseHeader &header,
                  const QByteArray &data );

signals:
   void request( QTcpSocket *id,
                 const QHttpRequestHeader &header );

private:
   WebServer( const WebServer &that );
   WebServer &operator=( const WebServer &that );

   QObject      *mpParent;
   QTcpServer   *mpTcpServer;
   QTcpSocket   *mpTcpSocket;
   QDir         mWebDir;
};

/*! @} */

#endif
