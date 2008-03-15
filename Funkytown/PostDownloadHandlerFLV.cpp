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

#include "Trace.hpp"


PostDownloadHandlerFLV::PostDownloadHandlerFLV()
: PostDownloadHandler()
, mConvProcess()
{
}

PostDownloadHandlerFLV::~PostDownloadHandlerFLV()
{
}

void PostDownloadHandlerFLV::run( const QString &/*url*/, const QString &filename, bool success )
{
   QFileInfo fileinfo( filename );
   if( success && (fileinfo.size() > 0) )
   {
      MySettings settings;
      qulonglong total = settings.value( "Bytes", 0 ).toULongLong();
      total += fileinfo.size();
      settings.setValue( "Bytes", total );
      
      unsigned int count = settings.value( "Files", 0 ).toUInt()+1;
      settings.setValue( "Files", count );
      
      settings.sendNotification( QString("f0v\n") + 
                                 QDir::currentPath() + '/' + filename );
      
      QString flv2mpeg4( settings.value( "flv2mpeg4", QString() ).toString() );
      QStringList args( filename );
      args.append( QString( filename ).replace( ".flv", ".avi" ) );
      
      mConvProcess.start( flv2mpeg4, args );
   }
}
