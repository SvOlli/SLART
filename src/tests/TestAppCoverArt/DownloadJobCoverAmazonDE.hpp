/*
 * src/tests/TestAppWebServer/DownloadJobCoverAmazonDE.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef DOWNLOADJOBCOVERAMAZONDE_HPP
#define DOWNLOADJOBCOVERAMAZONDE_HPP DOWNLOADJOBCOVERAMAZONDE_HPP

/* base class */
#include <QObject>

/* system headers */

/* Qt headers */
#include <QStringList>
#include <QUrl>
#include <QVariant>

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */
class QString;

/* forward declaration of local classes */
class Downloader;


class DownloadJobCoverAmazonDE : public QObject
{
   Q_OBJECT

public:
   DownloadJobCoverAmazonDE( Downloader *downloader,
                             QObject *target, const char *slot,
                             QObject *parent = 0 );
   virtual ~DownloadJobCoverAmazonDE();

   void query( const QString &query );

public slots:
   void parseHtml( const QByteArray &data, const QVariant &payload );

signals:
   void requestDownload( QObject *object, const char *slot,
                         const QUrl &urlList, const QVariant &payload );

private:
   Q_DISABLE_COPY( DownloadJobCoverAmazonDE )

   QObject        *mpTarget;
   const char     *mpSlot;
};

#endif
