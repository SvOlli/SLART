/*
 * src/apps/Rubberbandman/DirWalkerDelete.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "DirWalkerDelete.hpp"

/* system headers */

/* Qt headers */

/* local library headers */
#include <DatabaseInterface.hpp>

/* local headers */


DirWalkerDelete::DirWalkerDelete()
: mpDatabase( DatabaseInterface::get() )
, mDir()
{
}


DirWalkerDelete::~DirWalkerDelete()
{
}


void DirWalkerDelete::handleStart()
{
}


void DirWalkerDelete::handleFile( const QFileInfo &fileInfo )
{
   QFile::remove( fileInfo.absoluteFilePath() );
   mpDatabase->deleteTrackInfo( fileInfo.absoluteFilePath() );
}


void DirWalkerDelete::handleDirEntry( const QFileInfo & )
{
}


void DirWalkerDelete::handleDirLeave( const QFileInfo &fileInfo )
{
   mDir.rmdir( fileInfo.absoluteFilePath() );
}


void DirWalkerDelete::handleOther( const QFileInfo &fileInfo )
{
   QFile::remove( fileInfo.absoluteFilePath() );
}


void DirWalkerDelete::handleEnd()
{
}
