/**
 * PostDownloadHandlerFLV.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#ifndef POSTDOWNLOADHANDLERFLV_HPP
#define POSTDOWNLOADHANDLERFLV_HPP POSTDOWNLOADHANDLERFLV_HPP

#include "PostDownloadHandler.hpp"


class PostDownloadHandlerFLV : public PostDownloadHandler
{
public:
   PostDownloadHandlerFLV();
   
   /* analyse downloaded file */
   void run( const QString &url, const QString &filename, bool success, bool enqueue );
   
private:
   PostDownloadHandlerFLV( const PostDownloadHandlerFLV &other );
   PostDownloadHandlerFLV &operator=( const PostDownloadHandlerFLV &other );
};

#endif
