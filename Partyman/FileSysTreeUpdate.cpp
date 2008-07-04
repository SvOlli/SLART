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
   mpTreeModel->clear();
   return mpDatabase->getTrackInfoList( &trackInfoList );
}


void FileSysTreeUpdate::run()
{
   QString fileName;
   int i /*, size = 0*/;

   for( i = 0; i < trackInfoList.size(); i++ )
   {
      fileName = trackInfoList.at(i).mDirectory;
      fileName.append( "/" );
      fileName.append( trackInfoList.at(i).mFileName );
      mpTreeModel->addModelData( fileName );
   }
   trackInfoList.clear();
}
