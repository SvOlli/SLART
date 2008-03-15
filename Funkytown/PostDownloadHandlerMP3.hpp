/**
 * PostDownloadHandlerMP3.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#ifndef _POSTDOWNLOADHANDLERMP3_HPP_
#define _POSTDOWNLOADHANDLERMP3_HPP_ _POSTDOWNLOADHANDLERMP3_HPP_

#include "PostDownloadHandler.hpp"

class PostDownloadHandlerMP3 : public PostDownloadHandler
{
public:
   PostDownloadHandlerMP3();
   virtual ~PostDownloadHandlerMP3();
   void run( const QString &url, const QString &filename, bool success );

private:
   PostDownloadHandlerMP3( const PostDownloadHandlerMP3 &other );
   PostDownloadHandlerMP3 &operator=( const PostDownloadHandlerMP3 &other );
};

#endif
