/**
 * PostDownloadHandlerHTML.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */
 
#ifndef _POSTDOWNLOADHANDLERHTML_
#define _POSTDOWNLOADHANDLERHTML_ _POSTDOWNLOADHANDLERHTML_

#include "PostDownloadHandler.hpp"

class PostDownloadHandlerHTML : public PostDownloadHandler
{
public:
   PostDownloadHandlerHTML();
   virtual ~PostDownloadHandlerHTML();
   void run( const QString &url, const QString &filename, bool success );
   void setHandlers( DownloadHandler     *downloadHandler,
                     PostDownloadHandler *postDownloadHandler,
                     PostDownloadHandler *postDownloadHandlerFLV );

protected:
   PostDownloadHandler *mpPostDownloadHandlerFLV;

private:
   PostDownloadHandlerHTML( const PostDownloadHandlerHTML &other );
   PostDownloadHandlerHTML &operator=( const PostDownloadHandlerHTML &other );
};

#endif
