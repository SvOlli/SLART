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
#include <QObject>

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
 \brief \todo complete documentation complete documentation

 \dotfile "graphs/libs/Common/DatabaseThread_connect.dot" "Connect Graph"
*/
class DatabaseThread : public QObject
{
   Q_OBJECT

public:
   /*!
    \brief constructor

    \param fileName
   */
   DatabaseThread( const QString &fileName = QString() );
   /*!
    \brief destructor

   */
   virtual ~DatabaseThread();

   /*!
    \brief register Satellite for automatic notifications on database commits

    \param satellite object to send to (should be of class Satellite, 0 = disable)
    \param message message to send upon commit
   */
   void registerUpdate( QObject *satellite, const QByteArray &message );

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

   /*!
    \brief disable the next satellite notify

   */
   Q_INVOKABLE void disableNotify();

   /*!
    \brief get track information from database by id or filename

    \param target QObject to call on completion
    \param method with signature void method(TrackInfo[,QVariant])
    \param id
    \param fileName
    \param payload optional data to pass through to method
   */
   Q_INVOKABLE void getTrackInfo( QObject *target, const QByteArray &method,
                                  int id, const QString &fileName,
                                  const QVariant &payload = QVariant() );

   /*!
    \brief update track information to database

   */
   Q_INVOKABLE void updateTrackInfo( const TrackInfo &trackInfo, bool allowinsert = false );

   /*!
    \brief delete track information from database

   */
   Q_INVOKABLE void deleteTrackInfo( const TrackInfo &trackInfo );

   /*!
    \brief get a list of track information matching to the search string

    \param target QObject to call on completion
    \param method with signature void method(TrackInfoList[,QVariant])
    \param search
    \param payload optional data to pass through to method
   */
   Q_INVOKABLE void getTrackInfoList( QObject *target, const QByteArray &method,
                                      const QString &search = QString(),
                                      const QVariant &payload = QVariant() );

   /*!
    \brief get a list of file names matching to the search string

    \param target QObject to call on completion
    \param method with signature void method(QStringList[,QVariant])
    \param search
    \param payload optional data to pass through to method
   */
   Q_INVOKABLE void getPathNameList( QObject *target, const QByteArray &method,
                                     const QString &search = QString(),
                                     const QVariant &payload = QVariant() );

   /*!
    \brief get a random track

    \param target QObject to call on completion
    \param method with signature void method(TrackInfo[,QVariant])
    \param favorite
    \param leastplayed
    \param unwantedArtists
    \param folder
    \param payload optional data to pass through to method
   */
   Q_INVOKABLE void getRandomTrack( QObject *target, const QByteArray &method,
                                    bool favorite, bool leastplayed,
                                    const QStringList &excludeArtists = QStringList(),
                                    const QString &folder = QString(),
                                    const QVariant &payload = QVariant() );

   /*!
    \brief get a list of all available folders

    \param target QObject to call on completion
    \param method with signature void method(QStringList[,QVariant])
    \param payload optional data to pass through to method
   */
   Q_INVOKABLE void getFolders( QObject *target, const QByteArray &method,
                                const QVariant &payload = QVariant() );

   /*!
    \brief get all entries of a list
      QChar(1) = Favorite, QChar(2) = No Auto

    \param target QObject to call on completion
    \param method with signature void method(QStringList[,QVariant])
    \param folder
    \param payload optional data to pass through to method
   */
   Q_INVOKABLE void getFolder( QObject *target, const QByteArray &method,
                               const QString &folder,
                               const QVariant &payload = QVariant() );

   /*!
    \brief add a folder

   */
   Q_INVOKABLE void insertFolder( const QString &folder );

   /*!
    \brief remove a folder

   */
   Q_INVOKABLE void deleteFolder( const QString &folder );

   /*!
    \brief rename

   */
   Q_INVOKABLE void rename( const QString &oldName, const QString &newName );

   /*!
    \brief get all unique entries of a column

    \param target QObject to call on completion
    \param method with signature void method(QStringList[,QVariant])
    \param columnName
    \param payload optional data to pass through to method
   */
   Q_INVOKABLE void getAllColumnData( QObject *target, const QByteArray &method,
                                      const QString &columnName,
                                      const QVariant &payload = QVariant() );

   /*!
    \brief generate heavy load on database by creating a table, filling it with a
      lot of random data, dropping it and calling "VACUUM";

    \param target QObject to call on completion
    \param method with signature void method([QVariant])
    \param command test action to perform
    \param payload optional data to pass through to method
   */
   Q_INVOKABLE void generateTestLoad( QObject *target, const QByteArray &method,
                                      const QString &command, const QVariant &payload = QVariant() );

   /*!
    \brief for synchronization purposes

    \param target QObject to call
    \param method with signature void method([QVariant])
    \param payload optional data to pass through to method
   */
   Q_INVOKABLE void call( QObject *target, const QByteArray &method,
                          const QVariant &payload = QVariant() );

   /*!
    \brief \todo complete documentation

   */
   Q_INVOKABLE void commit( bool intermediate = false );

signals:
   /*!
    \brief \todo complete documentation

   */
   void working( bool on );

   /*!
    \brief \todo complete documentation

   */
   void sendUpdate( const QByteArray &message );

private:
   Q_DISABLE_COPY( DatabaseThread )

   /*!
    \brief \todo complete documentation

   */
   void prepare();

   /*!
    \brief send out an error description to Innuendo

    \param note \todo
   */
   void logError( const QString &note = QString() );

   /*!
    \brief \todo

    \param slot
    \return QByteArray
   */
   static QByteArray slotName( const QByteArray &slot );

   /*!
    \brief helper function for invoking a slot

    \param file for error message, pass on __FILE__ macro
    \param line for error message, pass on __LINE__ macro
    \param func for error message, pass on Q_FUNC_INFO macro
    \param target object to invoke
    \param method normalized slot to invoke
    \param payload optional payload to pass on
   */
   static void callSlot( const char *file, unsigned int line, const char *func,
                         QObject *target, const QByteArray &method,
                         const QVariant &payload = QVariant());
   /*!
    \brief helper function for invoking a slot

    \param file for error message, pass on __FILE__ macro
    \param line for error message, pass on __LINE__ macro
    \param func for error message, pass on Q_FUNC_INFO macro
    \param target object to invoke
    \param method normalized slot to invoke
    \param stringList data to send
    \param payload optional payload to pass on
   */
   static void callSlot( const char *file, unsigned int line, const char *func,
                         QObject *target, const QByteArray &method,
                         const QStringList &stringList,
                         const QVariant &payload = QVariant());
   /*!
    \brief helper function for invoking a slot

    \param file for error message, pass on __FILE__ macro
    \param line for error message, pass on __LINE__ macro
    \param func for error message, pass on Q_FUNC_INFO macro
    \param target object to invoke
    \param method normalized slot to invoke
    \param trackInfo data to send
    \param payload optional payload to pass on
   */
   static void callSlot( const char *file, unsigned int line, const char *func,
                         QObject *target, const QByteArray &method,
                         const TrackInfo &trackInfo,
                         const QVariant &payload = QVariant());
   /*!
    \brief helper function for invoking a slot

    \param file for error message, pass on __FILE__ macro
    \param line for error message, pass on __LINE__ macro
    \param func for error message, pass on Q_FUNC_INFO macro
    \param target object to invoke
    \param method normalized slot to invoke
    \param trackInfoList data to send
    \param payload optional payload to pass on
   */
   static void callSlot( const char *file, unsigned int line, const char *func,
                         QObject *target, const QByteArray &method,
                         const TrackInfoList &trackInfoList,
                         const QVariant &payload = QVariant());

   QSqlDatabase            *mpSqlDB;
   QSqlQuery               *mpQuery;
   QTimer                  *mpCommitTimer;
   QObject                 *mpSatellite;
   unsigned int            mDatabaseVersion;
   const unsigned int      mCodeVersion;
   int                     mUpdateCount;
   QByteArray              mUpdateMessage;
   bool                    mNotifyDisabled;
};

/*! @} */

#endif
