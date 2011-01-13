/**
 * src/apps/Rubberbandman/DirWalkerUpdate.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "DirWalkerUpdate.hpp"

/* system headers */

/* Qt headers */

/* local library headers */

/* local headers */
#include "DatabaseWorker.hpp"


DirWalkerUpdate::DirWalkerUpdate( DatabaseWorker *databaseWorker )
: mpDatabaseWorker( databaseWorker )
{
}


DirWalkerUpdate::~DirWalkerUpdate()
{
}


void DirWalkerUpdate::handleFile( const QFileInfo &fileInfo )
{
   mpDatabaseWorker->updateFile( fileInfo );
}


void DirWalkerUpdate::handleDirEntry( const QFileInfo &/*fileInfo*/ )
{
}


void DirWalkerUpdate::handleDirLeave( const QFileInfo &/*fileInfo*/ )
{
}


void DirWalkerUpdate::handleOther( const QFileInfo &/*fileInfo*/ )
{
}
