/**
 * FileSysTreeUpdate.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#include "FileSysTreeUpdate.hpp"
#include "FileSysTreeModel.hpp"
#include "Database.hpp"

#include <QtGui>

#include "Trace.hpp"


unsigned int FileSysTreeUpdate::prepare( Database *database, FileSysTreeModel *treeModel )
{
   mpDatabase  = database;
   mpTreeModel = treeModel;
   mCancel     = false;
   mpTreeModel->clear();
   return mpDatabase->getTrackInfoList( &trackInfoList );
}


void FileSysTreeUpdate::run()
{
   QString fileName;
   int i;

   for( i = 0; (i < trackInfoList.size()) && !mCancel; i++ )
   {
      mpTreeModel->addModelData( trackInfoList.at(i).filePath() );
   }
   trackInfoList.clear();
}
