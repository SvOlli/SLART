/*
 * src/apps/Rubberbandman/DirWalkerMove.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef DIRWALKERMOVE_HPP
#define DIRWALKERMOVE_HPP DIRWALKERMOVE_HPP

/* base class */
#include <DirWalker.hpp>

/* system headers */

/* Qt headers */
#include <QDir>
#include <QFileInfo>
#include <QString>

/* local library headers */
#include <TrackInfo.hpp>

/* local headers */

/* forward declaration of Qt classes */

/* forward declaration of local classes */
class DatabaseInterface;


/*!
  \addtogroup Rubberbandman

  @{
*/

/*!
 \brief TODO

*/
class DirWalkerMove : public DirWalkerCallbacks
{
public:
   /*!
    \brief constructor

    \param databaseWorker
   */
   DirWalkerMove( const QString &srcpath, const QString &destpath );

   /*!
    \brief destructor

   */
   virtual ~DirWalkerMove();

   /*!
    \brief called on start

   */
   virtual void handleStart();
   /*!
    \brief handle a file entry

   */
   virtual void handleFile( const QFileInfo &fileInfo );
   /*!
    \brief handle a directory entry upon entering

   */
   virtual void handleDirEntry( const QFileInfo &fileInfo );
   /*!
    \brief handle a directory entry upon leaving

   */
   virtual void handleDirLeave( const QFileInfo &fileInfo );
   /*!
    \brief handle an other entry (link, etc.)

   */
   virtual void handleOther( const QFileInfo &fileInfo );
   /*!
    \brief called on end

   */
   virtual void handleEnd();

private:
   Q_DISABLE_COPY( DirWalkerMove )

   DatabaseInterface    *mpDatabase;
   int                  mSrcBaseLen;
   QString              mDestBase;
   QString              mSrcFile;
   QString              mDestFile;
   QString              mPath;
   TrackInfo            mTrackInfo;
   QDir                 mQDir;
   QFileInfo            mQFileInfo;
};

/*! @} */

#endif
