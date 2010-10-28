/**
 * src/apps/Rubberbandman/DirWalkerMove.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "DirWalkerMove.hpp"

/* system headers */

/* Qt headers */
#include <QString>

/* local library headers */
#include <Database.hpp>

/* local headers */


DirWalkerMove::DirWalkerMove( Database *database, const QString &srcpath, const QString &destpath )
: mpDatabase( database )
, mSrcBaseLen( srcpath.length() )
, mDestBase( destpath )
, mSrcFile()
, mDestFile()
, mPath()
, mTrackInfo()
, mQDir()
, mQFileInfo()
{
//TRACESTART( DirWalkerMove )
//TRACEMSG << srcpath << destpath;
}


DirWalkerMove::~DirWalkerMove()
{
}


void DirWalkerMove::handleFile( const QFileInfo &fileInfo )
{
//TRACESTART( handleFile )
   mSrcFile = fileInfo.absoluteFilePath();
   mDestFile = mSrcFile;
   mDestFile.replace( 0, mSrcBaseLen, mDestBase );

//TRACEMSG << "rename" << mSrcFile << mDestFile;
   if( QFile::rename( mSrcFile, mDestFile ) )
   {
      if( mpDatabase->getTrackInfo( &mTrackInfo, mSrcFile ) )
      {
         mQFileInfo.setFile( mDestFile );
         mTrackInfo.mDirectory = mQFileInfo.absolutePath();
         mpDatabase->updateTrackInfo( &mTrackInfo, true );
      }
   }
}


void DirWalkerMove::handleDirEntry( const QFileInfo &fileInfo )
{
//TRACESTART( handleDirEntry )
   mDestFile = fileInfo.absoluteFilePath();
   mDestFile.replace( 0, mSrcBaseLen, mDestBase );
//TRACEMSG << "create" << mDestFile;
   mQDir.mkdir( mDestFile );
}


void DirWalkerMove::handleDirLeave( const QFileInfo &fileInfo )
{
//TRACESTART( handleDirEntry )
   mSrcFile = fileInfo.absoluteFilePath();
//TRACEMSG << "remove" << mSrcFile;

   mQDir.rmdir( mSrcFile );
}


void DirWalkerMove::handleOther( const QFileInfo &fileInfo )
{
   mSrcFile = fileInfo.absoluteFilePath();
   mDestFile = mSrcFile;
   mDestFile.replace( 0, mSrcBaseLen, mDestBase );
   QFile::rename( mSrcFile, mDestFile );
}
