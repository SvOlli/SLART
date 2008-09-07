/**
 * PostDownloadHandlerXML.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */
 
#ifndef POSTDOWNLOADHANDLERXML_HPP
#define POSTDOWNLOADHANDLERXML_HPP POSTDOWNLOADHANDLERXML_HPP

#include "PostDownloadHandler.hpp"


class PostDownloadHandlerXML : public PostDownloadHandler
{
public:
   PostDownloadHandlerXML();
   
   /* analyse downloaded file */
   void run( const QString &url, const QString &filename, bool success, bool enqueue );
   
private:
   PostDownloadHandlerXML( const PostDownloadHandlerXML &other );
   PostDownloadHandlerXML &operator=( const PostDownloadHandlerXML &other );
};

#endif
