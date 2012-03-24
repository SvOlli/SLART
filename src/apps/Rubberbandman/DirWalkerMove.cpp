/*
 * src/apps/Rubberbandman/DirWalkerMove.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "DirWalkerMove.hpp"

/* system headers */

/* Qt headers */
#include <QString>

/* local library headers */
#include <DatabaseInterface.hpp>

/* local headers */


DirWalkerMove::DirWalkerMove( const QString &srcpath, const QString &destpath )
: mpDatabase( DatabaseInterface::get() )
, mSrcBaseLen( srcpath.length() )
, mDestBase( destpath )
, mSrcFile()
, mDestFile()
, mPath()
, mTrackInfo()
, mQDir()
, mQFileInfo()
{
}


DirWalkerMove::~DirWalkerMove()
{
}


void DirWalkerMove::handleStart()
{
}


void DirWalkerMove::handleFile( const QFileInfo &fileInfo )
{
   mSrcFile = fileInfo.absoluteFilePath();
   mDestFile = mSrcFile;
   mDestFile.replace( 0, mSrcBaseLen, mDestBase );

   if( QFile::rename( mSrcFile, mDestFile ) )
   {
      mpDatabase->rename( mSrcFile, mDestFile );
   }
}


void DirWalkerMove::handleDirEntry( const QFileInfo &fileInfo )
{
   mDestFile = fileInfo.absoluteFilePath();
   mDestFile.replace( 0, mSrcBaseLen, mDestBase );
   mQDir.mkdir( mDestFile );
}


void DirWalkerMove::handleDirLeave( const QFileInfo &fileInfo )
{
   mSrcFile = fileInfo.absoluteFilePath();

   mQDir.rmdir( mSrcFile );
}


void DirWalkerMove::handleOther( const QFileInfo &fileInfo )
{
   mSrcFile = fileInfo.absoluteFilePath();
   mDestFile = mSrcFile;
   mDestFile.replace( 0, mSrcBaseLen, mDestBase );
   QFile::rename( mSrcFile, mDestFile );
}


void DirWalkerMove::handleEnd()
{
}
