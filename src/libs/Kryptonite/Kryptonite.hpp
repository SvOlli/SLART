/*
 * src/tests/TestAppWebServer/Kryptonite.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef KRYPTONITE_HPP
#define KRYPTONITE_HPP KRYPTONITE_HPP

/* base class */
#include <QObject>

/* system headers */

/* Qt headers */
#include <QIODevice>
#include <QList>
#include <QMap>
#include <QMutex>
#include <QPair>
#include <QTime>
#include <QUrl>
#include <QVariant>

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */
class QFile;
class QNetworkAccessManager;
class QNetworkCookieJar;
class QNetworkReply;
class QNetworkRequest;

/* forward declaration of local classes */


/*!
  \addtogroup Kryptonite

  \brief library: web download framework

  Named after [the song by 3 Doors Down](https://en.wikipedia.org/wiki/Kryptonite_%28song%29).

  @{
*/

/*!
 \brief class for generating download requests

 \dotfile "graphs/libs/Kryptonite/Kryptonite_connect.dot" "Connect Graph"
*/
class Kryptonite : public QObject
{
   Q_OBJECT

public:
   /*!
    \brief constructor

    \param parent
   */
   Kryptonite( QObject *parent = 0 );
   /*!
    \brief destructor

   */
   virtual ~Kryptonite();

   /*!
    \brief get the QNetworkAccessManager used for downloading

    necessary for settings proxy server

    \return QNetworkAccessManager
   */
   QNetworkAccessManager *networkAccessManager();

   /*!
    \brief remove all pending instances of \ref KryptoniteReply

   */
   void clear();

public slots:

   /*!
    \brief download data from webserver and send it to a slot

    \param target object to send data to
    \param slot slot to send data to, should be either (QByteArray) or (QByteArray,QVariant)
    \param url url to get data from
    \param payload payload to pass on to target object
   */
   QObject *download( QObject *target, const char *slot,
                      const QUrl &url, const QVariant &payload = QVariant() );
   /*!
    \brief download data from a webserver directly to a file

    unused right now

    \param fileName name of file to write data to
    \param url url to get data from
   */
   QObject *download( const QString &fileName, const QUrl &url );
   /*!
    \brief download data from a webserver directly to a QIODevice object

    unused right now

    \param file
    \param autoDelete delete file when transfer is complete
    \param url url to get data from
   */
   QObject *download( QIODevice *file, bool autoDelete, const QUrl &url );

   /*!
    \brief download data from webserver and send it to a slot

    \param target object to send data to
    \param slot slot to send data to, should be either (QByteArray) or (QByteArray,QVariant)
    \param request request to get data from
    \param payload payload to pass on to target object
   */
   QObject *download( QObject *target, const char *slot,
                      const QNetworkRequest &request, const QVariant &payload = QVariant() );
   /*!
    \brief download data from a webserver directly to a file

    unused right now

    \param fileName name of file to write data to
    \param request request to get data from
   */
   QObject *download( const QString &fileName, const QNetworkRequest &request );
   /*!
    \brief download data from a webserver directly to a QIODevice object

    unused right now

    \param file
    \param autoDelete delete file when transfer is complete
    \param request request to get data from
   */
   QObject *download( QIODevice *file, bool autoDelete, const QNetworkRequest &request );

signals:
   /*!
    \brief notify of a started download to harddisk

    \param id
    \param url
   */
   void downloadStarted( QObject *id, const QNetworkRequest &request );
   /*!
    \brief notify of a started download to harddisk

    \param id
    \param url
   */
   void downloadStarted( QObject *id, const QUrl &url );
   /*!
    \brief notify of progress on a download

    \param id
    \param percent
   */
   void downloadProgress( QObject *id, int percent );
   /*!
    \brief notify that download has finished

    \param id
   */
   void downloadFinished( QObject *id );

private:
   Q_DISABLE_COPY( Kryptonite )

   QNetworkAccessManager                  *mpManager; /*!< \brief \todo TODO */
   QNetworkCookieJar                      *mpCookieJar; /*!< \brief \todo TODO */
};

/*! @} */

#endif
