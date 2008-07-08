/**
 * PostDownloadHandlerFLV.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#ifndef _POSTDOWNLOADHANDLERFLV_HPP_
#define _POSTDOWNLOADHANDLERFLV_HPP_ _POSTDOWNLOADHANDLERFLV_HPP_

#include "PostDownloadHandler.hpp"

#include <QProcess>

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
