/*
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


/*!
  \addtogroup Rubberbandman

  @{
*/

/*!
 \brief TODO

*/
class DatabaseWorker : public QThread
{
   Q_OBJECT

public:
   /*!
    \brief constructor

   */
   DatabaseWorker();

   /*!
    \brief destructor

   */
   virtual ~DatabaseWorker();

   /*!
    \brief cancel current run

   */
   void cancel() { mCancel = true; }
   /*!
    \brief start update

   */
   void startUpdate( const QString &baseDir );
   /*!
    \brief start cleanup

   */
   void startCleanup();
   /*!
    \brief start import

   */
   void startImport( const QString &fileName );
   /*!
    \brief run the job

   */
   void run();

public slots:
   /*!
    \brief callback for update

   */
   void handleFile( const QFileInfo &fileInfo );
   /*!
    \brief callback for update

   */
   void handleEnd();
   /*!
    \brief read track info from the file

   */
   void updateTrackInfoFromFile( const TrackInfo &trackInfo );
   /*!
    \brief 

   */
   void cleanup( const TrackInfoList &trackInfoList );
   /*!
    \brief 

   */
   void updateStatus();

signals:
   /*!
    \brief emit progress

   */
   void progress( int checked, int processed );

private:
   Q_DISABLE_COPY( DatabaseWorker )

   /*!
    \brief subroutine for better reading of code

   */
   void importM3u();

   /*!
    \brief mode of operation

   */
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

/*! @} */

#endif
