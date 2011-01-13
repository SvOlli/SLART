/**
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


class DatabaseThread : public QThread
{
Q_OBJECT

public:
   DatabaseThread( const QString &fileName = QString() );
   virtual ~DatabaseThread();

   /*  */
   void run();

   /*  */
   void registerUpdate( Satellite *satellite, const QByteArray &message );

   /* generate the filename for DatabaseThread */
   static QString getDatabaseFileName();

private slots:

   /* disable the next satellite notify */
   void disableNotify();

   /* get track information from database by id or filename
      return call parameters: bool found, TrackInfo */
   void getTrackInfo( QObject *target, const QString &method,
                      int id, const QString &fileName );

   /* update track information to database */
   void updateTrackInfo( const TrackInfo &trackInfo, bool allowinsert = false );

   /* delete track information from database */
   void deleteTrackInfo( const TrackInfo &trackInfo );

   /* get a list of track information matching to the search string
      return call parameters: TrackInfoList */
   void getTrackInfoList( QObject *target, const QString &method,
                          const QString &search = QString() );

   /* get a list of file names matching to the search string
      return call parameters: QStringList */
   void getPathNameList( QObject *target, const QString &method,
                        const QString &search = QString() );

   /* get a random track */
   void getRandomTrack( QObject *target, const QString &method,
                        bool favorite, bool leastplayed,
                        const QString &folder = QString() );

   /* get a list of all available folders
      return call parameters: QStringList */
   void getFolders( QObject *target, const QString &method );

   /* get all entries of a list
      QChar(1) = Favorite, QChar(2) = No Auto
      return call parameters: QStringList */
   void getFolder( QObject *target, const QString &method,
                   const QString &folder );

   /* add a folder */
   void insertFolder( const QString &folder );

   /* remove a folder */
   void deleteFolder( const QString &folder );

   /* rename */
   void rename( const QString &oldName, const QString &newName );

   /* get all unique entries of a column
      return call parameters: QStringList */
   void getAllColumnData( QObject *target, const QString &method,
                          const QString &columnName );

   /* for synchronization purposes */
   void call( QObject *target, const QString &method );

   /*  */
   void commit( bool intermediate = false );

signals:
   /*  */
   void working( bool on );

private:
   DatabaseThread( const DatabaseThread &that );
   DatabaseThread &operator=( const DatabaseThread &that );

   /*  */
   void prepare();

   /* send out an error description to Innuendo */
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

#endif
