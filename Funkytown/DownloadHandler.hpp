/**
 * DownloadHandler.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#ifndef _DOWNLOADHANDLER_HPP_
#define _DOWNLOADHANDLER_HPP_ _DOWNLOADHANDLER_HPP_

#include <QWidget>
#include <QStringList>
#include "ProxyWidget.hpp"

class QHttp;
class QHttpResponseHeader;
class QFile;
class QProgressBar;
class QVBoxLayout;
class QTimer;
class QListWidget;

class PostDownloadHandler;

class DownloadHandler : public QWidget
{
Q_OBJECT

public:
   DownloadHandler( QWidget *parent = 0 );

   /* run a download request */
   void run( const QString &url, const QString &filename,
             PostDownloadHandler *postDownloadHandler );
   /* set the proxy configuration */
   void setProxy( ProxyWidget *proxyWidget );

public slots:
   /* callback for QHttp */
   void httpRequestFinished( int requestId, bool error );
   /* callback for QHttp */
   void readResponseHeader( const QHttpResponseHeader &responseHeader );
   /* update the progress bar */
   void updateDataReadProgress( int bytesRead, int totalBytes );
   /* start the download */
   void startDownload();

signals:
   /* signalize if download active or application is idle */
   void downloadActive( bool active );
   /* send a message if an error occured */
   void errorMessage( const QString &message );

private:
   DownloadHandler( const DownloadHandler &other );
   DownloadHandler &operator=( const DownloadHandler &other );

   bool                         mDownloading;
   int                          mHttpGetId;
   bool                         mAborting;

   QHttp                        *mpHttp;
   QFile                        *mpFile;
   QVBoxLayout                  *mpLayout;
   QProgressBar                 *mpProgressBar;
   QListWidget                  *mpQueue;
   QTimer                       *mpTimer;

   PostDownloadHandler          *mPostDownloadHandler;
   QList<PostDownloadHandler*>  mPostDownloadHandlers;
   QString                      mURL;
   QStringList                  mURLs;
   QString                      mFileName;
   QStringList                  mFileNames;
};

#endif
