/**
 * PostDownloadHandlerXML.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#include "PostDownloadHandlerXML.hpp"

#include <QFile>
#include <QStringList>
#include <QRegExp>


PostDownloadHandlerXML::PostDownloadHandlerXML()
: PostDownloadHandler()
{
}

PostDownloadHandlerXML::~PostDownloadHandlerXML()
{
}

void PostDownloadHandlerXML::run( const QString &url, const QString &filename, bool success )
{
   int i;
   
   if( !success )
   {
      return;
   }
   
   QFile file( filename );
   file.open( QIODevice::ReadOnly );
   QString data( QString::fromUtf8( file.readAll() ) );
   file.close();
   file.remove();
   
   bool quoton = false;
   bool cdataon = false;
   for( i = 0; i < data.size(); i++ )
   {
      if( data.mid(i,8) == QString("![CDATA[") )
      {
         cdataon = true;
      }
      switch(data.at(i).toAscii() )
      {
         case '"':
         quoton = !quoton;
         break;
         case ']':
         if( cdataon ) cdataon = false;
         break;
         case ' ':
         case '<':
         case '>':
         case '\r':
         if( !quoton && !cdataon )
         {
         data.replace(i,1,'\n');
         }
         break;
         default:
         // nothing
         break;
      }
   }
   
   QStringList dataList = data.split( "\n", QString::SkipEmptyParts );
   QString xml( data );
   QString xmlName;
   QString xmlTitle;
   QString xmlUrl( url ); // url just to keep the compiler warning-free
   for( i = 0; i < dataList.size(); i++ )
   {
      if( dataList.at(i) == QString("name") )
      {
         xmlName = dataList.at(i+1);
         xmlName.replace( QRegExp( "!\\[CDATA\\[(.*)\\]\\]" ), "\\1" );
         //TRACEMSG << "name" << name;
      }
      if( dataList.at(i) == QString("song") )
      {
         xmlTitle = "";
         xmlUrl   = "";
         //TRACEMSG << "song";
      }
      if( dataList.at(i).startsWith("title=") )
      {
         xmlTitle = dataList.at(i);
         xmlTitle.replace( QRegExp( ".*\"(.*)\".*" ), "\\1" );
         //TRACEMSG << "title" << title;
      }
      if( dataList.at(i).startsWith("curl=") ||
          dataList.at(i).startsWith("durl=") || 
          dataList.at(i).startsWith("purl=") || 
          dataList.at(i).startsWith("downloadable=") )
      {
         xmlUrl = dataList.at(i);
         if( xmlUrl.indexOf(".mp3") < 0 )
         {
            xmlUrl.clear();
         }
         else
         {
            xmlUrl.replace( QRegExp( ".*\"(.*)[\\?\"].*" ), "\\1" );
            xmlUrl.replace("std_","full_");
            xmlUrl.replace("&amp;","&");
         }
      }
      if( !xmlUrl.isEmpty() && !xmlTitle.isEmpty() )
      {
         QString filename( xmlName+QString(" - ")+xmlTitle+QString(".mp3") );
         filename.replace( QRegExp("[:/\\*\\?\\\\]"), "_" );
         mpDownloadHandler->run( xmlUrl, filename, mpPostDownloadHandler );
         xmlTitle = "";
         xmlUrl   = "";
      }
   }
}
