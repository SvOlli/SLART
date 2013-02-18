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
                             QObject *parent = 0 );
   virtual ~DownloadJobCoverAmazonDE();

   void setTarget(QObject *object, const char *slot );
   void query( const QString &query );
public slots:
   void parseHtml( const QList<QByteArray> &data );

signals:
   void requestDownload( QObject *object, const char *slot,
                         const QList<QUrl> &urlList );

private:
   Q_DISABLE_COPY( DownloadJobCoverAmazonDE )

   QObject        *mpObject;
   const char     *mpSlot;
};

#endif
