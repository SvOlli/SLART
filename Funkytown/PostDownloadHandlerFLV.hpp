/**
 * PostDownloadHandlerFLV.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#ifndef _POSTDOWNLOADHANDLERFLV_
#define _POSTDOWNLOADHANDLERFLV_ _POSTDOWNLOADHANDLERFLV_

#include "PostDownloadHandler.hpp"

#include <QProcess>

class PostDownloadHandlerFLV : public PostDownloadHandler
{
public:
   PostDownloadHandlerFLV();
   virtual ~PostDownloadHandlerFLV();
   void run( const QString &url, const QString &filename, bool success );

private:
   PostDownloadHandlerFLV( const PostDownloadHandlerFLV &other );
   PostDownloadHandlerFLV &operator=( const PostDownloadHandlerFLV &other );

   QProcess mConvProcess;
};

#endif
