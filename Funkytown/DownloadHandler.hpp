/**
 * DownloadHandler.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#ifndef DOWNLOADHANDLER_HPP
#define DOWNLOADHANDLER_HPP DOWNLOADHANDLER_HPP

#include <QWidget>

#include <QStringList>

#include "ProxyWidget.hpp"

class QHttp;
class QHttpResponseHeader;
class QListWidget;
class QProgressBar;
class QTimer;
class QVBoxLayout;

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
   ScrollLine                   *mpURL;
   ScrollLine                   *mpFileName;
   QProgressBar                 *mpProgressBar;
   QTimer                       *mpTimer;
   MagicQueue                   *mpMagicQueue;
   TheMagic                     *mpTheMagic;
};

#endif
