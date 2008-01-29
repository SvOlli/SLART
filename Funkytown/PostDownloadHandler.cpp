/**
 * PostDownloadHandler.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#include "PostDownloadHandler.hpp"


PostDownloadHandler::PostDownloadHandler()
: mpDownloadHandler( 0 )
, mpPostDownloadHandler( 0 )
{
}

PostDownloadHandler::~PostDownloadHandler()
{
}

void PostDownloadHandler::setHandlers( DownloadHandler *downloadHandler,
                                       PostDownloadHandler *postDownloadHandler )
{
   mpDownloadHandler     = downloadHandler;
   mpPostDownloadHandler = postDownloadHandler;
}
