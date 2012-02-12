/*
 * src/apps/Rubberbandman/DirWalkerUpdate.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef DIRWALKERUPDATE_HPP
#define DIRWALKERUPDATE_HPP DIRWALKERUPDATE_HPP

/* base class */
#include <DirWalker.hpp>

/* system headers */

/* Qt headers */
#include <QObject>

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */
class QFileInfo;

/* forward declaration of local classes */
class DatabaseWorker;


/*!
  \addtogroup Rubberbandman

  @{
*/

/*!
 \brief \todo complete documentation

*/
class DirWalkerUpdate : public DirWalkerCallbacks
{
public:
   /*!
    \brief constructor

    \param databaseWorker
   */
   DirWalkerUpdate( DatabaseWorker *databaseWorker );

   /*!
    \brief destructor

   */
   virtual ~DirWalkerUpdate();

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
   Q_DISABLE_COPY( DirWalkerUpdate )

   DatabaseWorker    *mpDatabaseWorker; /*!< \brief \todo complete documentation */
};

/*! @} */

#endif
