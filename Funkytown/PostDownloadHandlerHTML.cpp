/**
 * PostDownloadHandlerHTML.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#include "PostDownloadHandlerHTML.hpp"

#include <QString>
#include <QFile>

#include "Trace.hpp"


PostDownloadHandlerHTML::PostDownloadHandlerHTML()
: PostDownloadHandler()
{
}

PostDownloadHandlerHTML::~PostDownloadHandlerHTML()
{
}

void PostDownloadHandlerHTML::setHandlers( DownloadHandler     *downloadHandler,
                                           PostDownloadHandler *postDownloadHandler,
                                           PostDownloadHandler *postDownloadHandlerFLV )
{
   mpDownloadHandler        = downloadHandler;
   mpPostDownloadHandler    = postDownloadHandler;
   mpPostDownloadHandlerFLV = postDownloadHandlerFLV;
}

void PostDownloadHandlerHTML::run( const QString &/*url*/, const QString &filename, bool success )
{
   if( !success )
   {
      return;
   }
   
   QString friendID;
   QString videoID;
   QString t;
   QString title;
   QString embed;
   QFile file( filename );
   QString line;
   QString mySpace("DisplayFriendId\":");
   QString youTube("fullscreenUrl");
   QString param("http://www.youtube.com/v/");
   QString colon(",");
   int pos;
   int endPos;
   
   if( !file.open( QIODevice::ReadOnly ) )
   {
      return;
   }
   
   while( !file.atEnd() )
   {
      line = QString::fromUtf8( file.readLine() );
      pos = line.indexOf( mySpace, 0, Qt::CaseInsensitive );
      if( pos >= 0 )
      {
         pos += mySpace.size();
         endPos = line.indexOf( colon, pos );
         if( endPos >= 0 )
         {
            friendID = line.mid( pos, endPos-pos );
            break;
         }
      }
      pos = line.indexOf( youTube, 0, Qt::CaseInsensitive );
      if( pos >= 0 )
      {
         QStringList parts( line.split( "&" ) );
         for( pos = 0; pos < parts.count(); pos++ )
         {
            if( parts.at(pos).startsWith( "t=" ) )
            {
               t=parts.at(pos);
            }
            
            if( parts.at(pos).startsWith( "video_id=" ) )
            {
               videoID=parts.at(pos);
            }
            
            if( parts.at(pos).startsWith( "title=" ) )
            {
               title=parts.at(pos).left(parts.at(pos).lastIndexOf("'")).mid(6);
            }
         }
      }
      pos = line.indexOf( param, 0, Qt::CaseInsensitive );
      if( pos > 0 )
      {
         embed = line;
         embed.replace( QRegExp(".*\"http://"), "http://" );
         embed.remove( QRegExp("&.*") );
         embed.replace( "/v/", "/watch?v=" );
      }
   }
   file.remove();
   file.close();
   
   if( !friendID.isEmpty() )
   {
      mpDownloadHandler->run( QString("http://mediaservices.myspace.com/services/media/musicplayerxml.ashx?b=")
                                     +friendID,
                              friendID+QString(".xml"),
                              mpPostDownloadHandler );
   }
   
   if( !videoID.isEmpty() && !t.isEmpty() && !title.isEmpty() )
   {
      mpDownloadHandler->run( QString("http://youtube.com/get_video?")+videoID+"&"+t,
                              title+QString(".flv"),
                              mpPostDownloadHandlerFLV );
   }
   else if( !embed.isEmpty() )
   {
      mpDownloadHandler->run( embed, "youtube.html", this );
   }
}
