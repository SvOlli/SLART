/*
 * src/libs/Common/DirWalker.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

/* class declaration */
#include "DirWalker.hpp"

/* system headers */

/* Qt headers */
#include <QDir>
#include <QFileInfoList>

/* local library headers */

/* local headers */


DirWalker::DirWalker()
{
}


DirWalker::~DirWalker()
{
}


void DirWalker::run( DirWalkerCallbacks *callbacks,
                     const QString &directoryPath )
{
   callbacks->handleStart();
   runDir( callbacks, directoryPath );
   callbacks->handleEnd();
}


void DirWalker::runDir( DirWalkerCallbacks *callbacks,
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
         runDir( callbacks, entries.at(i).absoluteFilePath() );
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
