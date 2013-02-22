/*
 * src/apps/Karmadrome/ExportFolderList.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "ExportFolderList.hpp"

/* system headers */
#include <cstdio>

/* Qt headers */
#include <QFile>

/* local library headers */
#include <DatabaseInterface.hpp>

/* local headers */

#include "Trace.hpp"
#include <QTimer>

ExportFolderList::ExportFolderList( const QString &fileName )
: QObject( 0 )
, mFileName( fileName )
{
   DatabaseInterface *database = DatabaseInterface::get();

   database->getFolders( this, SLOT(writeData(QStringList)) );
}


ExportFolderList::~ExportFolderList()
{
}


void ExportFolderList::writeData( const QStringList &entries )
{
   QFile file( mFileName );
   const QByteArray lf( "\n" );

   if( mFileName.isEmpty() )
   {
      file.open( ::stdout, QIODevice::WriteOnly );
   }
   else
   {
      if( !file.open( QIODevice::WriteOnly | QIODevice::Text ) )
      {
         return;
      }
   }
   foreach( const QString &entry, entries )
   {
      file.write( entry.toLocal8Bit() );
      file.write( lf );
   }
   file.close();
   deleteLater();
}
