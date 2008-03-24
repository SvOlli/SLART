/**
 * PostDownloadHandlerHTML.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#include "PostDownloadHandlerHTML.hpp"
#include "GlobalHandlers.hpp"

#include <QString>
#include <QFile>

#include "Trace.hpp"


PostDownloadHandlerHTML::PostDownloadHandlerHTML()
: PostDownloadHandler()
{
}


void PostDownloadHandlerHTML::run( const QString &url, const QString &filename, bool success )
{
   if( !success )
   {
      return;
   }
   
   if( url.contains( QRegExp( "^http://[^/]*myspace.com/", Qt::CaseInsensitive, QRegExp::RegExp2 ) ) )
   {
      runMySpace( url, filename );
   }
   else if( url.contains( QRegExp( "^http://[^/]*youtube.com/", Qt::CaseInsensitive, QRegExp::RegExp2 ) ) )
   {
      runYouTube( url, filename );
   }
   else
   {
      runGeneric( url, filename );
   }

}


void PostDownloadHandlerHTML::runYouTube( const QString &/*url*/, const QString &filename )
{
   int pos;
   QString line;
   QString youTube("fullscreenUrl");
   QString t;
   QString title;
   QString videoID;
   
   QFile file( filename );
   if( !file.open( QIODevice::ReadOnly ) )
   {
      return;
   }
   
   while( !file.atEnd() )
   {
      line = QString::fromUtf8( file.readLine() );
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
   }
   file.remove();
   file.close();
   
   if( !videoID.isEmpty() && !t.isEmpty() && !title.isEmpty() )
   {
      gpDownloadHandler->run( QString("http://youtube.com/get_video?")+videoID+"&"+t,
                              title+QString(".flv"),
                              gpPostDownloadHandlerFLV );
   }
}


void PostDownloadHandlerHTML::runMySpace( const QString &/*url*/, const QString &filename )
{
   int pos;
   int endPos;
   QString mySpace("DisplayFriendId\":");
   QString colon(",");
   QString friendID;
   QString line;
   
   QFile file( filename );
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
   }
   file.remove();
   file.close();
   
   if( !friendID.isEmpty() )
   {
      gpDownloadHandler->run( QString("http://mediaservices.myspace.com/services/media/musicplayerxml.ashx?b=")
                                     +friendID,
                              friendID+QString(".xml"),
                              gpPostDownloadHandlerXML );
   }
}


void PostDownloadHandlerHTML::runGeneric( const QString &url, const QString &filename )
{
   int pos;
   QString param("http://www.youtube.com/v/");
   QString embed;
   QString line;
   QRegExp href( "href=.*mp3", Qt::CaseInsensitive );
   
   QFile file( filename );
   if( !file.open( QIODevice::ReadOnly ) )
   {
      return;
   }
   
   while( !file.atEnd() )
   {
      line = QString::fromUtf8( file.readLine() );
      pos = line.indexOf( param, 0, Qt::CaseInsensitive );
      if( pos > 0 )
      {
         QString embed( line );
         embed.replace( QRegExp(".*\"http://"), "http://" );
         embed.remove( QRegExp("&.*") );
         embed.replace( "/v/", "/watch?v=" );
         gpDownloadHandler->run( embed, "youtube.html", gpPostDownloadHandlerHTML );
      }
      pos = line.indexOf( href );
      if( pos > 0 )
      {
         QString mp3ref( line );
         mp3ref.remove( QRegExp("^.*href=['\"]*") );
         mp3ref.remove( QRegExp("\\.mp3['\"]*.*$") );
         mp3ref.append( ".mp3" );
         
         if( !mp3ref.startsWith( "http://" ) )
         {
            if( mp3ref.startsWith( "/" ) )
            {
               mp3ref.prepend( url.left( url.indexOf( '/', 8 ) ) );
            }
            else
            {
               mp3ref.prepend( url.left( url.lastIndexOf( '/' ) + 1 ) );
            }
         }
         QString filename( mp3ref );
         filename.remove( QRegExp( "^.*/" ) );
         gpDownloadHandler->run( mp3ref, filename, gpPostDownloadHandlerMP3 );
      }
   }
   file.remove();
   file.close();
}
