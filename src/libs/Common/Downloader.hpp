/*
 * src/tests/TestAppWebServer/Downloader.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef DOWNLOADER_HPP
#define DOWNLOADER_HPP DOWNLOADER_HPP

/* base class */
#include <QObject>

/* system headers */

/* Qt headers */
#include <QList>
#include <QMap>
#include <QMutex>
#include <QPair>
#include <QTime>
#include <QUrl>
#include <QVariant>

/* local library headers */
#include <TrackInfo.hpp>

/* local headers */

/* forward declaration of Qt classes */
class QFile;
class QNetworkAccessManager;
class QNetworkCookieJar;
class QNetworkReply;

/* forward declaration of local classes */


/*!
 \brief class for generating download requests

*/
class Downloader : public QObject
{
   Q_OBJECT

public:
   /*!
    \brief constructor

    \param parent
   */
   Downloader( QObject *parent = 0 );
   /*!
    \brief destructor

   */
   virtual ~Downloader();

public slots:

   /*!
    \brief download data from webserver and send it to a slot

    \param target object to send data to
    \param slot slot to send data to, should be either (QByteArray) or (QByteArray,QVariant)
    \param url url to get data from
    \param payload payload to pass on to target object
   */
   void download( QObject *target, const char *slot,
                  const QUrl &url, const QVariant &payload = QVariant() );
   /*!
    \brief download data from a webserver directly to a file

    unused right now

    \param fileName name of file to write data to
    \param url url to get data from
   */
   void download( const QString &fileName, const QUrl &url );
   /*!
    \brief download data from a webserver directly to a QIODevice object

    unused right now

    \param file
    \param url url to get data from
    \param autoDelete delete file when transfer is complete
   */
   void download( QIODevice *file, const QUrl &url, bool autoDelete = false );

signals:

private:
   Q_DISABLE_COPY( Downloader )

   QNetworkAccessManager                  *mpManager; /*!< \brief \todo TODO */
   QNetworkCookieJar                      *mpCookieJar; /*!< \brief \todo TODO */
};

#endif
