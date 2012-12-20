/*
 * src/apps/Partyman/FileSysTreeUpdate.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "FileSysTreeUpdate.hpp"

/* system headers */

/* Qt headers */
#include <QtGui>

/* local library headers */
#include <Database.hpp>

/* local headers */
#include "FileSysTreeModel.hpp"


int FileSysTreeUpdate::prepare( Database *database, FileSysTreeModel *treeModel )
{
   mpDatabase  = database;
   mpTreeModel = treeModel;
   mCancel     = false;
   mpTreeModel->clear();
   return mpDatabase->getTrackInfoList( &mTrackInfoList );
}


void FileSysTreeUpdate::run()
{
   foreach( const TrackInfo &trackInfo, mTrackInfoList )
   {
      mpTreeModel->addModelData( trackInfo.filePath() );
   }
   mTrackInfoList.clear();
}
