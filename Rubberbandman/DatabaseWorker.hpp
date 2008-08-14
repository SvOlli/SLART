/**
 * DatabaseWorker.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#ifndef DATABASEWORKER_HPP
#define DATABASEWORKER_HPP DATABASEWORKER_HPP

#include <QThread>
#include <QString>
#include "TrackInfo.hpp"
#include "DirWalker.hpp"

class QFileInfo;
class FileSysTreeModel;
class Database;

class DatabaseWorker : public QThread
{
Q_OBJECT
   
public:
   /*  */
   void prepare( Database *database );
   /*  */
   void cancel() { mCancel = true; } ;
   /*  */
   bool initUpdate( const QString &baseDir );
   /*  */
   bool initCleanup();
   /*  */
   bool initImport( const QString &fileName );
   /*  */
   void run();

   /*  */
   void updateFile( const QFileInfo &fileInfo );
   /*  */
   void updateDir( const QFileInfo &fileInfo );
   
signals:
   void progress( int checked, int processed );

private:
   /*  */
   bool updateTrackInfoFromFile( const QString &fileName );
   /*  */
   void importM3u();

   enum { none, update, cleanup, import } mMode;
   Database         *mpDatabase;
   FileSysTreeModel *mpTreeModel;
   bool             mCancel;
   int              mChecked;
   int              mLastChecked;
   int              mProcessed;
   QString          mPath;
   DirWalker        mDirWalker;
   TrackInfo        mTrackInfo;
};

#endif
