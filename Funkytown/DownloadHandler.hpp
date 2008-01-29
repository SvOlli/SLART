/**
 * DownloadHandler.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#ifndef _DOWNLOADHANDLER_
#define _DOWNLOADHANDLER_ _DOWNLOADHANDLER_

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
   virtual ~DownloadHandler();
   void run( const QString &url, const QString &filename,
             PostDownloadHandler *postDownloadHandler );
   void setProxy( ProxyWidget *proxyWidget );

public slots:
   void httpRequestFinished( int requestId, bool error );
   void readResponseHeader( const QHttpResponseHeader &responseHeader );
   void updateDataReadProgress( int bytesRead, int totalBytes );
   void startDownload();

signals:
   void downloadActive( bool active );
   void errorMessage( const QString &message );

private:

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
