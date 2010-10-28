/**
 * src/apps/Rubberbandman/DirWalkerDelete.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "DirWalkerDelete.hpp"

/* system headers */

/* Qt headers */

/* local library headers */
#include <Database.hpp>

/* local headers */


DirWalkerDelete::DirWalkerDelete( Database *database )
: mpDatabase( database )
, mDir()
, mTrackInfo()
{
}


DirWalkerDelete::~DirWalkerDelete()
{
}


void DirWalkerDelete::handleFile( const QFileInfo &fileInfo )
{
   QFile::remove( fileInfo.absoluteFilePath() );
   if( mpDatabase->getTrackInfo( &mTrackInfo, fileInfo.absoluteFilePath() ) )
   {
      mpDatabase->deleteTrackInfo( &mTrackInfo );
   }
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
