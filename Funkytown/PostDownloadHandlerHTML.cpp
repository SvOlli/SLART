/**
 * PostDownloadHandlerHTML.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#include "PostDownloadHandlerHTML.hpp"

#include <QString>
#include <QFile>


PostDownloadHandlerHTML::PostDownloadHandlerHTML()
: PostDownloadHandler()
{
}

PostDownloadHandlerHTML::~PostDownloadHandlerHTML()
{
}

void PostDownloadHandlerHTML::run( const QString &url, const QString &filename, bool success )
{
   if( !success )
   {
      return;
   }
   
   QString friendID;
   QFile file( filename );
   QString line( url ); // url just to keep the compiler warning-free
   QString searchFor("DisplayFriendId\":");
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
      pos = line.indexOf( searchFor, 0, Qt::CaseInsensitive );
      if( pos >= 0 )
      {
         pos += searchFor.size();
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
      mpDownloadHandler->run( QString("http://mediaservices.myspace.com/services/media/musicplayerxml.ashx?b=")
                                     +friendID,
                              friendID+QString(".xml"),
                              mpPostDownloadHandler );
   }
}
