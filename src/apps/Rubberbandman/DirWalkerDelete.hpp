/*
 * src/apps/Rubberbandman/DirWalkerDelete.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef DIRWALKERDELETE_HPP
#define DIRWALKERDELETE_HPP DIRWALKERDELETE_HPP

/* base class */
#include <DirWalker.hpp>

/* system headers */

/* Qt headers */
#include <QDir>

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */
class QFileInfo;

/* forward declaration of local classes */
class DatabaseInterface;


/*!
  \addtogroup Rubberbandman

  @{
*/

/*!
 \brief \todo complete documentation

*/
class DirWalkerDelete : public DirWalkerCallbacks
{
public:
   /*!
    \brief constructor

   */
   DirWalkerDelete();

   /*!
    \brief destructor

   */
   virtual ~DirWalkerDelete();

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
   Q_DISABLE_COPY( DirWalkerDelete )

   DatabaseInterface    *mpDatabase;
   QDir                 mDir;
};

/*! @} */

#endif
