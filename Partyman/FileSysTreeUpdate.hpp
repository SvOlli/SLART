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

class Database;
class FileSysTreeModel;


class FileSysTreeUpdate : public QThread
{
public:
   /* prepare the update function (constructor replacement) */
   int prepare( Database *database, FileSysTreeModel *treeModel );
   /* cancel current operation */
   void cancel() { mCancel = true; } ;
   /* start main work (called by QThread) */
   void run();
   
private:
   Database         *mpDatabase;
   FileSysTreeModel *mpTreeModel;
   bool             mCancel;
   TrackInfoList    mTrackInfoList;
};

#endif
