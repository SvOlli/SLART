/*
 * src/libs/Common/DatabaseThread.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef DATABASETHREAD_HPP
#define DATABASETHREAD_HPP DATABASETHREAD_HPP

/* base class */
#include <QThread>

/* system headers */

/* Qt headers */
#include <QStringList>

/* local library headers */

/* local headers */
#include "TrackInfo.hpp"

/* forward declaration of Qt classes */
class QSqlDatabase;
class QSqlQuery;

/* forward declaration of local classes */
class Satellite;


/*!
  \addtogroup Common

  @{
*/

/*!
 \brief TODO

*/
class DatabaseThread : public QThread
{
   Q_OBJECT

public:
   DatabaseThread( const QString &fileName = QString() );
   virtual ~DatabaseThread();

   /*!
    \brief 

   */
   void run();

   /*!
    \brief 

   */
   void registerUpdate( Satellite *satellite, const QByteArray &message );

   /*!
    \brief generate the filename for DatabaseThread

   */
   static QString getDatabaseFileName();

private slots:

   /*!
    \brief disable the next satellite notify

   */
   void disableNotify();

   /*!
    \brief get track information from database by id or filename
      return call parameters: bool found, TrackInfo

    \param target
    \param method
    \param id
    \param fileName
   */
   void getTrackInfo( QObject *target, const QString &method,
                      int id, const QString &fileName );

   /*!
    \brief update track information to database

   */
   void updateTrackInfo( const TrackInfo &trackInfo, bool allowinsert = false );

   /*!
    \brief delete track information from database

   */
   void deleteTrackInfo( const TrackInfo &trackInfo );

   /*!
    \brief get a list of track information matching to the search string
      return call parameters: TrackInfoList

    \param target
    \param method
    \param search
   */
   void getTrackInfoList( QObject *target, const QString &method,
                          const QString &search = QString() );

   /*!
    \brief get a list of file names matching to the search string
      return call parameters: QStringList

    \param target
    \param method
    \param search
   */
   void getPathNameList( QObject *target, const QString &method,
                        const QString &search = QString() );

   /*!
    \brief get a random track

   */
   void getRandomTrack( QObject *target, const QString &method,
                        bool favorite, bool leastplayed,
                        const QString &folder = QString() );

   /*!
    \brief get a list of all available folders
      return call parameters: QStringList

    \param target
    \param method
   */
   void getFolders( QObject *target, const QString &method );

   /*!
    \brief get all entries of a list
      QChar(1) = Favorite, QChar(2) = No Auto
      return call parameters: QStringList

    \param target
    \param method
    \param folder
   */
   void getFolder( QObject *target, const QString &method,
                   const QString &folder );

   /*!
    \brief add a folder

   */
   void insertFolder( const QString &folder );

   /*!
    \brief remove a folder

   */
   void deleteFolder( const QString &folder );

   /*!
    \brief rename

   */
   void rename( const QString &oldName, const QString &newName );

   /*!
    \brief get all unique entries of a column
      return call parameters: QStringList

    \param target
    \param method
    \param columnName
   */
   void getAllColumnData( QObject *target, const QString &method,
                          const QString &columnName );

   /*!
    \brief for synchronization purposes

   */
   void call( QObject *target, const QString &method );

   /*!
    \brief 

   */
   void commit( bool intermediate = false );

signals:
   /*!
    \brief 

   */
   void working( bool on );

private:
   Q_DISABLE_COPY( DatabaseThread )

   /*!
    \brief 

   */
   void prepare();

   /*!
    \brief send out an error description to Innuendo

   */
   void logError( const QString &note = QString() );

   QSqlDatabase            *mpSqlDB;
   QSqlQuery               *mpQuery;
   QTimer                  *mpCommitTimer;
   Satellite               *mpSatellite;
   unsigned int            mDatabaseVersion;
   const unsigned int      mCodeVersion;
   int                     mUpdateCount;
   QByteArray              mUpdateMessage;
   bool                    mNotifyDisabled;
};

/*! @} */

#endif
