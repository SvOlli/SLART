/**
 * FileSysTreeUpdate.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#ifndef FILESYSTREEUPDATE_HPP
#define FILESYSTREEUPDATE_HPP FILESYSTREEUPDATE_HPP

#include <QThread>
#include "TrackInfo.hpp"

class FileSysTreeModel;
class Database;

class FileSysTreeUpdate : public QThread
{
public:
   unsigned int prepare( Database *database, FileSysTreeModel *treeModel );
   void run();

private:
   Database         *mpDatabase;
   FileSysTreeModel *mpTreeModel;
   TrackInfoList    trackInfoList;
};

#endif
