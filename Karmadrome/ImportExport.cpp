/**
 * ImportExport.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 */

#include "ImportExport.hpp"
#include "Database.hpp"
#include "TrackInfo.hpp"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QString>

#include <iostream>

#include "Trace.hpp"


ImportExport::ImportExport( Database *database )
: mpDatabase( database )
{
   qsrand( time((time_t*)0) );
}


void ImportExport::importM3u( QString folder, const QString &fileName, bool cleanImport )
{
   TrackInfo trackInfo;
   QChar favorite(1);
   QChar unwanted(2);
   
   if( folder.startsWith( "|F", Qt::CaseInsensitive ) )
   {
      folder = favorite;
   }
   if( folder.startsWith( "|U", Qt::CaseInsensitive ) )
   {
      folder = unwanted;
   }
   
   if( cleanImport )
   {
      QStringList entries( folder );
      mpDatabase->beginTransaction();
      for( int i = 0; i < entries.count(); i++ )
      {
         if( mpDatabase->getTrackInfo( &trackInfo, entries.at(i) ) )
         {
            if( folder == favorite )
            {
               trackInfo.setFlag( TrackInfo::Favorite, false );
            }
            else if( folder == unwanted )
            {
               trackInfo.setFlag( TrackInfo::Unwanted, false );
            }
            else
            {
               trackInfo.setFolder( folder, false );
            }
            mpDatabase->updateTrackInfo( &trackInfo );
         }
      }
      mpDatabase->endTransaction( true );
   }
   
   QFile m3uFile( fileName );
   if( m3uFile.open( QIODevice::ReadOnly | QIODevice::Text ) )
   {
      QString fileName;
      QString fileBase( m3uFile.fileName() + "/../" );
      QFileInfo qfi;
      mpDatabase->beginTransaction();
      while( !m3uFile.atEnd() )
      {
         fileName = QString::fromLocal8Bit( m3uFile.readLine() );
         if( !fileName.startsWith("#") )
         {
            if( fileName.right(1) == QChar('\n') )
            {
               fileName.chop(1);
            }
            if( !fileName.startsWith( "/" ) )
            {
               /* a bit of an ugly trick, but gets the job done better than most
                  other solutions */
               qfi.setFile( fileBase + fileName );
               fileName = qfi.absoluteFilePath();
            }
            if( mpDatabase->getTrackInfo( &trackInfo, fileName ) )
            {
               if( folder == favorite )
               {
                  trackInfo.setFlag( TrackInfo::Favorite, true );
               }
               else if( folder == unwanted )
               {
                  trackInfo.setFlag( TrackInfo::Unwanted, true );
               }
               else
               {
                  trackInfo.setFolder( folder, true );
               }
               mpDatabase->updateTrackInfo( &trackInfo );
            }
         }
      }
      mpDatabase->endTransaction( true );
      m3uFile.close();
   }
}


void ImportExport::exportM3u( QString folder, const QString &fileName, bool relative, bool randomize )
{
   QFile m3uFile( fileName );
   QChar favorite(1);
   QChar unwanted(2);
   
   if( folder.startsWith( "|F", Qt::CaseInsensitive ) )
   {
      folder = favorite;
   }
   if( folder.startsWith( "|U", Qt::CaseInsensitive ) )
   {
      folder = unwanted;
   }
   
   if( !m3uFile.open( QIODevice::WriteOnly | QIODevice::Text ) )
   {
      return;
   }
   QDir dir( QFileInfo( m3uFile.fileName() ).absolutePath() );
   QStringList entries( mpDatabase->getFolder( folder ) );
   if( randomize )
   {
      QStringList randomized;
      while( entries.count() )
      {
         randomized.append( entries.takeAt( qrand() % entries.count() ) );
      }
      entries = randomized;
   }
   if( relative )
   {
      for( int i = 0; i < entries.count(); i++ )
      {
         m3uFile.write( dir.relativeFilePath( entries.at(i) ).toLocal8Bit() );
         m3uFile.write( "\n" );
      }
   }
   else
   {
      for( int i = 0; i < entries.count(); i++ )
      {
         m3uFile.write( entries.at(i).toLocal8Bit() );
         m3uFile.write( "\n" );
      }
   }
   m3uFile.close();
}


void ImportExport::listFolders( const QString &fileName )
{
   QStringList folders( mpDatabase->getFolders() );
   QFile file( fileName );
   QString lf( "\n" );

   if( !fileName.isEmpty() )
   {
      if( !file.open( QIODevice::WriteOnly | QIODevice::Text ) )
      {
         return;
      }
   }
   for( int i = 0; i < folders.size(); i++ )
   {
      if( fileName.isEmpty() )
      {
         std::cout << folders.at(i).toLocal8Bit().constData() << std::endl;
      }
      else
      {
         file.write( folders.at(i).toLocal8Bit() );
         file.write( lf.toLocal8Bit() );
      }
   }
   if( !fileName.isEmpty() )
   {
      file.close();
   }
}
