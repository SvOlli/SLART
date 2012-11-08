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
#include <QDir>
#include <QStringList>
#include <QVariant>

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
 \brief \todo complete documentation

 \dotfile "graphs/libs/Common/DatabaseThread_connect.dot" "Connect Graph"
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
   static inline QString getDatabaseFileName()
   {
#ifdef Q_OS_WIN32
      QString slartdb( "%1/slart.db" );
#else
      QString slartdb( "%1/.slartdb" );
#endif
      return slartdb.arg( QDir::homePath() );
   }


private slots:

   /*!
    \brief disable the next satellite notify

   */
   void disableNotify();

   /*!
    \brief get track information from database by id or filename

    \param target QObject to call on completion
    \param method with signature void method(bool,const TrackInfo &[,const QVariant &])
    \param id
    \param fileName
    \param payload optional data to pass through to method
   */
   void getTrackInfo( QObject *target, const QString &method,
                      int id, const QString &fileName,
                      const QVariant &payload = QVariant() );

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

    \param target QObject to call on completion
    \param method with signature void method(const TrackInfoList &[,const QVariant &])
    \param search
    \param payload optional data to pass through to method
   */
   void getTrackInfoList( QObject *target, const QString &method,
                          const QString &search = QString(),
                          const QVariant &payload = QVariant() );

   /*!
    \brief get a list of file names matching to the search string

    \param target QObject to call on completion
    \param method with signature void method(const QStringList &[,const QVariant &])
    \param search
    \param payload optional data to pass through to method
   */
   void getPathNameList( QObject *target, const QString &method,
                         const QString &search = QString(),
                         const QVariant &payload = QVariant() );

   /*!
    \brief get a random track

    \param target QObject to call on completion
    \param method with signature void method(const TrackInfo &[,const QVariant &])
    \param favorite
    \param leastplayed
    \param unwantedArtists
    \param folder
    \param payload optional data to pass through to method
   */
   void getRandomTrack( QObject *target, const QString &method,
                        bool favorite, bool leastplayed,
                        const QStringList &excludeArtists = QStringList(),
                        const QString &folder = QString(),
                        const QVariant &payload = QVariant() );

   /*!
    \brief get a list of all available folders

    \param target QObject to call on completion
    \param method with signature void method(const QStringList &[,const QVariant &])
    \param payload optional data to pass through to method
   */
   void getFolders( QObject *target, const QString &method,
                    const QVariant &payload = QVariant() );

   /*!
    \brief get all entries of a list
      QChar(1) = Favorite, QChar(2) = No Auto

    \param target QObject to call on completion
    \param method with signature void method(const QStringList &[,const QVariant &])
    \param folder
    \param payload optional data to pass through to method
   */
   void getFolder( QObject *target, const QString &method,
                   const QString &folder,
                   const QVariant &payload = QVariant() );

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

    \param target QObject to call on completion
    \param method with signature void method(const QStringList &[,const QVariant &])
    \param columnName
    \param payload optional data to pass through to method
   */
   void getAllColumnData( QObject *target, const QString &method,
                          const QString &columnName,
                          const QVariant &payload = QVariant() );

   /*!
    \brief generate heavy load on database by creating a table, filling it with a
      lot of random data, dropping it and calling "VACUUM";

    \param target QObject to call on completion
    \param method with signature void method([const QVariant &])
    \param command test action to perform
    \param payload optional data to pass through to method
   */
   void generateTestLoad( QObject *target, const QString &method,
                          const QString &command, const QVariant &payload = QVariant() );

   /*!
    \brief for synchronization purposes

    \param target QObject to call
    \param method with signature void method([const QVariant &])
    \param payload optional data to pass through to method
   */
   void call( QObject *target, const QString &method,
              const QVariant &payload = QVariant() );

   /*!
    \brief \todo

   */
   void commit( bool intermediate = false );

signals:
   /*!
    \brief \todo

   */
   void working( bool on );

private:
   Q_DISABLE_COPY( DatabaseThread )

   /*!
    \brief \todo

   */
   void prepare();

   /*!
    \brief send out an error description to Innuendo

    \param note \todo
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
