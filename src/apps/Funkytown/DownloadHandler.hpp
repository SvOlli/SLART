/**
 * src/apps/Funkytown/DownloadHandler.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef DOWNLOADHANDLER_HPP
#define DOWNLOADHANDLER_HPP DOWNLOADHANDLER_HPP

/* base class */
#include <QWidget>

/* system headers */

/* Qt headers */
#include <QStringList>

/* local library headers */
#include <ProxyWidget.hpp>

/* local headers */
#include "CookieJar.hpp"

/* forward declaration of Qt classes */
class QCheckBox;
class QHttp;
class QHttpResponseHeader;
class QListWidget;
class QProgressBar;
class QTimer;
class QVBoxLayout;

/* forward declaration of local classes */
class MagicQueue;
class ScrollLine;
class TheMagic;


class DownloadHandler : public QWidget
{
Q_OBJECT

public:
   DownloadHandler( QWidget *parent = 0 );

   /* run a download request */
   void run( const QString &url );

public slots:
   /* callback for QHttp */
   void httpRequestFinished( int requestId, bool error );
   /* callback for QHttp */
   void readResponseHeader( const QHttpResponseHeader &responseHeader );
   /* update the progress bar */
   void updateDataReadProgress( int bytesRead, int totalBytes );
   /* start the download */
   void startDownload();
   /* handle change of the enqueue checkbox */
   void handleEnqueue( bool checked );

signals:
   /* signalize if download active or application is idle */
   void downloadActive( bool active );
   /* send a message if an error occured */
   void errorMessage( const QString &message );

private:
   DownloadHandler( const DownloadHandler &that );
   DownloadHandler &operator=( const DownloadHandler &that );

   bool                         mDownloading;
   int                          mHttpGetId;
   bool                         mAborting;

   QHttp                        *mpHttp;
   ScrollLine                   *mpURL;
   ScrollLine                   *mpFileName;
   QCheckBox                    *mpEnqueue;
   QProgressBar                 *mpProgressBar;
   QTimer                       *mpTimer;
   MagicQueue                   *mpMagicQueue;
   TheMagic                     *mpTheMagic;
   CookieJar                    mCookieJar;
};

#endif
