/**
 * src/apps/Rubberbandman/DatabaseWorker.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef DATABASEWORKER_HPP
#define DATABASEWORKER_HPP DATABASEWORKER_HPP

/* base class */
#include <QThread>

/* system headers */

/* Qt headers */
#include <QString>
#include <QTime>

/* local library headers */
#include <TrackInfo.hpp>
#include <DirWalker.hpp>

/* local headers */

/* forward declaration of Qt classes */
class QFileInfo;

/* forward declaration of local classes */
class DatabaseInterface;
class FileSysTreeModel;


class DatabaseWorker : public QThread
{
   Q_OBJECT

public:
   DatabaseWorker();
   virtual ~DatabaseWorker();

   /* cancel current run */
   void cancel() { mCancel = true; }
   /* start update */
   void startUpdate( const QString &baseDir );
   /* start cleanup */
   void startCleanup();
   /* start import */
   void startImport( const QString &fileName );
   /* run the job */
   void run();

public slots:
   /* callback for update */
   void handleFile( const QFileInfo &fileInfo );
   /* callback for update */
   void handleEnd();
   /* read track info from the file */
   void updateTrackInfoFromFile( const TrackInfo &trackInfo );
   /*  */
   void cleanup( const TrackInfoList &trackInfoList );
   /*  */
   void updateStatus();

signals:
   /* emit progress */
   void progress( int checked, int processed );

private:
   DatabaseWorker( const DatabaseWorker &that );
   DatabaseWorker &operator=( const DatabaseWorker &that );

   /* subroutine for better reading of code */
   void importM3u();

   enum { ModeNone, ModeUpdate, ModeCleanup, ModeImport } mMode;
   DatabaseInterface    *mpDatabase;
   bool                 mCancel;
   int                  mChecked;
   int                  mProcessed;
   QString              mPath;
   DirWalker            mDirWalker;
   TrackInfo            mTrackInfo;
   QTime                mTime;
};

#endif
