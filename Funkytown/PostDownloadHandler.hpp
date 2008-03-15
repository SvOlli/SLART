/**
 * PostDownloadHandler.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#ifndef _POSTDOWNLOADHANDLER_
#define _POSTDOWNLOADHANDLER_ _POSTDOWNLOADHANDLER_

#include "DownloadHandler.hpp"

class QString;

class PostDownloadHandler
{
public:
   PostDownloadHandler();
   virtual ~PostDownloadHandler();
   virtual void run( const QString &url, const QString &filename, bool success ) = 0;

private:
   PostDownloadHandler( const PostDownloadHandler &other );
   PostDownloadHandler &operator=( const PostDownloadHandler &other );
};

#endif
