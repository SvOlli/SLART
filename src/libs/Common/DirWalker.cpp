/**
 * src/libs/Common/DirWalker.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#include "DirWalker.hpp"

#include "Trace.hpp"

#include <QDir>
#include <QFileInfoList>

DirWalker::DirWalker()
{
}


DirWalker::~DirWalker()
{
}


void DirWalker::run( DirWalkerCallbacks *callbacks,
                     const QString &directoryPath )
{
   QDir dir( directoryPath );
   QFileInfoList entries( dir.entryInfoList( QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot ) );
   int i;
   
   for( i = 0; i < entries.count(); i++ )
   {
      if( entries.at(i).isDir() )
      {
         callbacks->handleDirEntry( entries.at(i) );
         run( callbacks, entries.at(i).absoluteFilePath() );
         callbacks->handleDirLeave( entries.at(i) );
      }
      else if( entries.at(i).isFile() )
      {
         callbacks->handleFile( entries.at(i) );
      }
      else
      {
         callbacks->handleOther( entries.at(i) );
      }
   }
}
