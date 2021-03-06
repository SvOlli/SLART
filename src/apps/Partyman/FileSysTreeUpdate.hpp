/*
 * src/apps/Partyman/FileSysTreeUpdate.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef FILESYSTREEUPDATE_HPP
#define FILESYSTREEUPDATE_HPP FILESYSTREEUPDATE_HPP

/* base class */
#include <QThread>

/* system headers */

/* Qt headers */

/* local library headers */
#include <TrackInfo.hpp>

/* local headers */

/* forward declaration of Qt classes */

/* forward declaration of local classes */
class Database;
class FileSysTreeModel;


/*!
  \addtogroup Partyman

  @{
*/

/*!
 \brief \todo complete documentation

*/
class FileSysTreeUpdate : public QThread
{
public:
   /*!
    \brief prepare the update function (constructor replacement)

   */
   int prepare( Database *database, FileSysTreeModel *treeModel );
   /*!
    \brief cancel current operation

    unimplemented
   */
   void cancel() { mCancel = true; }
   /*!
    \brief start main work (called by QThread)

   */
   void run();

private:
   Database         *mpDatabase;
   FileSysTreeModel *mpTreeModel;
   bool             mCancel;
   TrackInfoList    mTrackInfoList;
};

/*! @} */

#endif
