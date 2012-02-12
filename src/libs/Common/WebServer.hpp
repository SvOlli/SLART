/*
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
 \brief \todo complete documentation

 \dotfile "graphs/libs/Common/WebServer_connect.dot" "Connect Graph"
*/
class WebServer : public QObject
{
   Q_OBJECT

public:

   /*!
    \brief constructor

    \param parent
   */
   WebServer( QObject *parent = 0 );

   /*!
    \brief destructor

   */
   virtual ~WebServer();

   /*!
    \brief \todo complete documentation

    \param port
    \param webPath
   */
   bool start( quint16 port = 0, const QString &webPath = QString() );

   /*!
    \brief \todo complete documentation

   */
   void stop();

public slots:
   /*!
    \brief

   */
   void handleNewConnection();

   /*!
    \brief \todo complete documentation

    \param id
    \param header
    \param data
   */
   void response( QTcpSocket *id,
                  const QHttpResponseHeader &header,
                  const QByteArray &data );

signals:
   /*!
    \brief \todo complete documentation

    \param id
    \param header
   */
   void request( QTcpSocket *id,
                 const QHttpRequestHeader &header );

private:
   Q_DISABLE_COPY( WebServer )

   QObject      *mpParent;
   QTcpServer   *mpTcpServer;
   QTcpSocket   *mpTcpSocket;
   QDir         mWebDir;
};

/*! @} */

#endif
