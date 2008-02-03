/**
 * PostDownloadHandlerMP3.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#include "PostDownloadHandlerMP3.hpp"

#include <QString>
#include <QFileInfo>
#include <QDir>
#include "MySettings.hpp"


PostDownloadHandlerMP3::PostDownloadHandlerMP3()
: PostDownloadHandler()
{
}

PostDownloadHandlerMP3::~PostDownloadHandlerMP3()
{
}

void PostDownloadHandlerMP3::run( const QString &url, const QString &filename, bool success )
{
   QString full_("/full_");
   
   QFileInfo fileinfo( filename );
   if( !fileinfo.size() )
   {
      success = false;
   }
   else
   {
      MySettings settings;
      qulonglong total = settings.value( "Bytes", 0 ).toULongLong();
      total += fileinfo.size();
      settings.setValue( "Bytes", total );
      
      unsigned int count = settings.value( "Files", 0 ).toUInt()+1;
      settings.setValue( "Files", count );
      
      settings.sendNotification( QString("f0d\n") + 
                                 QDir::currentPath() + '/' + filename );
   }
   
   if( !success && url.indexOf( full_ ) >= 0 )
   {
      QString newurl(url);
      QString std_("/std_");
      newurl.replace( full_, std_ );
      mpDownloadHandler->run( newurl, filename, mpPostDownloadHandler );
   }
}
