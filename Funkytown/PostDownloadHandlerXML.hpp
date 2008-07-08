/**
 * PostDownloadHandlerXML.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */
 
#ifndef _POSTDOWNLOADHANDLERXML_HPP_
#define _POSTDOWNLOADHANDLERXML_HPP_ _POSTDOWNLOADHANDLERXML_HPP_

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
