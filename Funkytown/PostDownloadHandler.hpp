/**
 * PostDownloadHandler.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#ifndef POSTDOWNLOADHANDLER_HPP
#define POSTDOWNLOADHANDLER_HPP POSTDOWNLOADHANDLER_HPP

#include "DownloadHandler.hpp"

class QString;


class PostDownloadHandler
{
public:
   PostDownloadHandler();
   virtual ~PostDownloadHandler();
   
   /* analyse downloaded file */
   virtual void run( const QString &url, const QString &filename, bool success, bool enqueue ) = 0;
   
private:
   PostDownloadHandler( const PostDownloadHandler &other );
   PostDownloadHandler &operator=( const PostDownloadHandler &other );
};

#endif
