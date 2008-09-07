/**
 * PostDownloadHandlerMP3.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#ifndef POSTDOWNLOADHANDLERMP3_HPP
#define POSTDOWNLOADHANDLERMP3_HPP POSTDOWNLOADHANDLERMP3_HPP

#include "PostDownloadHandler.hpp"


class PostDownloadHandlerMP3 : public PostDownloadHandler
{
public:
   PostDownloadHandlerMP3();
   
   /* analyse downloaded file */
   void run( const QString &url, const QString &filename, bool success, bool enqueue );
   
private:
   PostDownloadHandlerMP3( const PostDownloadHandlerMP3 &other );
   PostDownloadHandlerMP3 &operator=( const PostDownloadHandlerMP3 &other );
};

#endif
