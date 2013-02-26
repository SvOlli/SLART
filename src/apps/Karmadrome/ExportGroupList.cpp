/*
 * src/apps/Karmadrome/ExportGroupList.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "ExportGroupList.hpp"

/* system headers */
#include <cstdio>

/* Qt headers */
#include <QFile>

/* local library headers */
#include <DatabaseInterface.hpp>

/* local headers */

#include "Trace.hpp"
#include <QTimer>

ExportGroupList::ExportGroupList( const QString &fileName )
: QObject( 0 )
, mFileName( fileName )
{
   DatabaseInterface *database = DatabaseInterface::get();

   database->getGroups( this, SLOT(writeData(QStringList)) );
}


ExportGroupList::~ExportGroupList()
{
}


void ExportGroupList::writeData( const QStringList &entries )
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
