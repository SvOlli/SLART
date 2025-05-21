/*
 * src/apps/Rubberbandman/DatabaseWorker.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef DATABASEWORKER_HPP
#define DATABASEWORKER_HPP DATABASEWORKER_HPP

/* base class */
#include <QObject>

/* system headers */

/* Qt headers */
#include <QElapsedTimer>
#include <QString>

/* local library headers */
#include <TrackInfo.hpp>

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
 \brief \todo complete documentation

*/
class DatabaseWorker : public QObject
{
   Q_OBJECT

public:
   /*!
    \brief constructor

    \param parent
   */
   DatabaseWorker( QObject *parent = 0 );

   /*!
    \brief destructor

   */
   virtual ~DatabaseWorker();

   /*!
    \brief \todo

    \param path
   */
   void setPath( const QString &path );

public slots:
   /*!
    \brief start update

    \param baseDir
   */
   void startUpdate();
   /*!
    \brief start cleanup

   */
   void startCleanup();
   /*!
    \brief start import

    \param fileName
   */
   void startImport();

   /*!
    \brief callback for update

    \param fileInfo
   */
   void handleFile( const QFileInfo &fileInfo );
   /*!
    \brief callback for update

   */
   void handleEnd();
   /*!
    \brief read track info from the file

    \param trackInfo
   */
   void updateTrackInfoFromFile( const TrackInfo &trackInfo );
   /*!
    \brief 

    \param trackInfoList
   */
   void cleanup( const TrackInfoList &trackInfoList );
   /*!
    \brief 

   */
   void updateStatus();

signals:
   /*!
    \brief send out a message

    \param msg
   */
   void message( const QString &msg );

   /*!
    \brief emit progress

    \param checked
    \param processed
   */
   void progress( int checked, int processed );

   /*!
    \brief current job is done

   */
   void done();

private:
   Q_DISABLE_COPY( DatabaseWorker )

   DatabaseInterface    *mpDatabase; /*!< \brief handle to database */
   int                  mDepth;  /*!< \brief current depth in directory tree */
   int                  mChecked; /*!< \brief number of items that where checked */
   int                  mProcessed; /*!< \brief number of items that were processed */
   QElapsedTimer        mTime; /*!< \brief internal timer to check when to update */
   QString              mPath; /*!< \brief path/file to start with */
};

/*! @} */

#endif
