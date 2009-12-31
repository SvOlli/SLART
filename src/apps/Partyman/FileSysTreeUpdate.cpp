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
   QString fileName;
   int i;

   for( i = 0; (i < mTrackInfoList.size()) && !mCancel; i++ )
   {
      mpTreeModel->addModelData( mTrackInfoList.at(i).filePath() );
   }
   mTrackInfoList.clear();
}