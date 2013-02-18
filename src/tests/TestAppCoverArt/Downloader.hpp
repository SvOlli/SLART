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

/* local library headers */
#include <TrackInfo.hpp>

/* local headers */

/* forward declaration of Qt classes */
class QFile;
class QNetworkAccessManager;
class QNetworkCookieJar;
class QNetworkReply;

/* forward declaration of local classes */


class Downloader : public QObject
{
   Q_OBJECT

public:
   Downloader( QObject *parent = 0 );
   virtual ~Downloader();

public slots:
#if 0
   void download( QFile *file, const QUrl &url, bool autoDelete = false );
   void download( const QString &fileName, const QUrl &url );
#endif
   void download(QObject *target, const char *slot,
                  const QList<QUrl> &urlList );

   void handleFinished( QNetworkReply* reply );

signals:
   void done( const QList<QByteArray> &dataList );

private:
   Q_DISABLE_COPY( Downloader )

   QNetworkAccessManager                  *mpManager;
   QNetworkCookieJar                      *mpCookieJar;
   QMap<QNetworkReply*,unsigned int>      mReplyMap;
   QMap<unsigned int,QObject*>            mObjects;
   QMap<unsigned int,const char*>         mSlots;
   QMap<unsigned int,QList<QByteArray> >  mDataMap;
};

#endif
