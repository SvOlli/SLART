/**
 * PostDownloadHandlerXML.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */
 
#ifndef _POSTDOWNLOADHANDLERXML_
#define _POSTDOWNLOADHANDLERXML_ _POSTDOWNLOADHANDLERXML_

#include "PostDownloadHandler.hpp"

class PostDownloadHandlerXML : public PostDownloadHandler
{
public:
   PostDownloadHandlerXML();
   virtual ~PostDownloadHandlerXML();
   void run( const QString &url, const QString &filename, bool success );

private:
   PostDownloadHandlerXML( const PostDownloadHandlerXML &other );
   PostDownloadHandlerXML &operator=( const PostDownloadHandlerXML &other );
};

#endif
