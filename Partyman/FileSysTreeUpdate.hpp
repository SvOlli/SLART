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
   int prepare( Database *database, FileSysTreeModel *treeModel );
   void cancel() { mCancel = true; } ;
   void run();

private:
   Database         *mpDatabase;
   FileSysTreeModel *mpTreeModel;
   bool             mCancel;
   TrackInfoList    trackInfoList;
};

#endif
