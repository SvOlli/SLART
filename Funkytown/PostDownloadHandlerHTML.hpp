/**
 * PostDownloadHandlerHTML.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */
 
#ifndef _POSTDOWNLOADHANDLERHTML_HPP_
#define _POSTDOWNLOADHANDLERHTML_HPP_ _POSTDOWNLOADHANDLERHTML_HPP_

#include "PostDownloadHandler.hpp"

class PostDownloadHandlerHTML : public PostDownloadHandler
{
public:
   PostDownloadHandlerHTML();

   /* analyse downloaded file */
   void run( const QString &url, const QString &filename, bool success );

private:
   PostDownloadHandlerHTML( const PostDownloadHandlerHTML &other );
   PostDownloadHandlerHTML &operator=( const PostDownloadHandlerHTML &other );

   /* run download for generic page */
   void runGeneric( const QString &url, const QString &filename );
   /* run download for MySpace page */
   void runMySpace( const QString &url, const QString &filename );
   /* run download for YouTube page */
   void runYouTube( const QString &url, const QString &filename );
};

#endif
