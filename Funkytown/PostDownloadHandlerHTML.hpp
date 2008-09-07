/**
 * PostDownloadHandlerHTML.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */
 
#ifndef POSTDOWNLOADHANDLERHTML_HPP
#define POSTDOWNLOADHANDLERHTML_HPP POSTDOWNLOADHANDLERHTML_HPP

#include "PostDownloadHandler.hpp"


class PostDownloadHandlerHTML : public PostDownloadHandler
{
public:
   PostDownloadHandlerHTML();
   
   /* analyse downloaded file */
   void run( const QString &url, const QString &filename, bool success, bool enqueue );
   
private:
   PostDownloadHandlerHTML( const PostDownloadHandlerHTML &other );
   PostDownloadHandlerHTML &operator=( const PostDownloadHandlerHTML &other );
   
   /* run download for generic page */
   void runGeneric( const QString &url, const QString &filename );
   /* run download for MySpace page */
   void runMySpace( const QString &url, const QString &filename );
   /* run download for YouTube page */
   void runYouTube( const QString &url, const QString &filename );
   
   bool mEnqueue;
};

#endif
