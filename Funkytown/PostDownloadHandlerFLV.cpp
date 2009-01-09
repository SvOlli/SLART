/**
 * PostDownloadHandlerFLV.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#include "PostDownloadHandlerFLV.hpp"

#include <QString>
#include <QFileInfo>
#include <QDir>
#include "MySettings.hpp"
#include "ConfigDialog.hpp"

#include "Trace.hpp"


PostDownloadHandlerFLV::PostDownloadHandlerFLV()
: PostDownloadHandler()
{
}


void PostDownloadHandlerFLV::run( const QString &/*url*/, const QString &filename,
                                  bool success, bool /*enqueue*/ )
{
   QFileInfo fileinfo( filename );
   if( success && (fileinfo.size() > 0) )
   {
      MySettings settings;
      qulonglong total = settings.VALUE_BYTES;
      total += fileinfo.size();
      settings.setValue( "Bytes", total );
      
      unsigned int count = settings.VALUE_FILES + 1;
      settings.setValue( "Files", count );
      
      settings.sendNotification( QString("f0v\n") + 
                                 QDir::currentPath() + '/' + filename );
   }
}
