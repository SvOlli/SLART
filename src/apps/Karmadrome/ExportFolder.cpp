/*
 * src/apps/Karmadrome/ExportFolder.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "ExportFolder.hpp"

/* system headers */
#include <cstdio>

/* Qt headers */
#include <QDir>
#include <QFile>
#include <QString>

/* local library headers */
#include <DatabaseInterface.hpp>

/* local headers */


ExportFolder::ExportFolder( const QString &folder, const QString &fileName,
                            bool relative, bool randomize )
: QObject( 0 )
, mRelative( relative )
, mRandomize( randomize )
, mFileName( fileName )
{
   qsrand( time((time_t*)0) );
   DatabaseInterface *database = DatabaseInterface::get();

   if( folder.startsWith( "|F", Qt::CaseInsensitive ) )
   {
      database->getFolder( this, SLOT(writeData(QStringList)), QString( QChar(1) ) );
   }
   else if( folder.startsWith( "|U", Qt::CaseInsensitive ) )
   {
      database->getFolder( this, SLOT(writeData(QStringList)), QString( QChar(2) ) );
   }
   else
   {
      database->getFolder( this, SLOT(writeData(QStringList)), folder );
   }
}


ExportFolder::~ExportFolder()
{
}


void ExportFolder::writeData( const QStringList &entries )
{
   QFile m3uFile( mFileName );
   QDir dir;
   const QByteArray lf("\n");
   QStringList randomized;
   const QStringList *e = &entries;

   if( mFileName.isEmpty() )
   {
      m3uFile.open( ::stdout, QIODevice::WriteOnly );
   }
   else
   {
      dir.cd( QFileInfo( m3uFile.fileName() ).absolutePath() );
      if( !m3uFile.open( QIODevice::WriteOnly | QIODevice::Text ) )
      {
         return;
      }
   }

   if( mRandomize )
   {
      QList<int> ids;
      for( int i = 0; i < entries.count(); i++ )
      {
         ids << i;
      }
      while( ids.count() )
      {
         randomized << entries.at( ids.takeAt( qrand() % ids.count() ) );
      }
      e = &randomized;
   }
   if( mRelative )
   {
      foreach( const QString &entry, *e )
      {
         m3uFile.write( dir.relativeFilePath( entry ).toLocal8Bit() );
         m3uFile.write( lf );
      }
   }
   else
   {
      foreach( const QString &entry, *e )
      {
         m3uFile.write( entry.toLocal8Bit() );
         m3uFile.write( lf );
      }
   }
   m3uFile.close();
   deleteLater();
}
