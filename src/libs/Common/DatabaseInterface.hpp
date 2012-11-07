/*
 * src/libs/Common/DatabaseInterface.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef DATABASEINTERFACE_HPP
#define DATABASEINTERFACE_HPP DATABASEINTERFACE_HPP

/* base class */

/* system headers */

/* Qt headers */
#include <QFileInfo>
#include <QStringList>

/* local library headers */

/* local headers */
#include "DatabaseThread.hpp"
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

*/
class DatabaseInterface
{
public:
   /*!
    \brief 

    \return DatabaseInterface *
   */
   static DatabaseInterface *create();
   /*!
    \brief 

   */
   static void destroy();
   /*!
    \brief 

    \return DatabaseInterface *
   */
   static DatabaseInterface *get();

   /*!
    \brief test for the existance of the Database file

    \return bool
   */
   static inline bool exists()
   {
      return QFileInfo( DatabaseThread::getDatabaseFileName() ).isFile();
   }

   /*!
    \brief connect activity indicator

    \param object that will receive messages
    \param slot needs to be in form of "method(bool)"
   */
   void connectActivityIndicator( QObject *object, const char *slot );

   /*!
    \brief disconnect previous connected activity indicator

    \param object that will receive messages
    \param slot needs to be in form of "method(bool)"
   */
   void disconnectActivityIndicator( QObject *object, const char *slot );

   /*!
    \brief 

    \param satellite
    \param message
   */
   void registerUpdate( Satellite *satellite, const QByteArray &message );

   /*!
    \brief disable the next satellite notify

   */
   void disableNotify();

   /*!
    \brief get track information from database by id

    \param target QObject to call on completion
    \param method with signature void method(bool,const TrackInfo &[,const QVariant &])
    \param id
    \param payload optional data to pass through to method
   */
   void getTrackInfo( QObject *target, const QString &method,
                      int id,
                      const QVariant &payload = QVariant() );

   /*!
    \brief get track information from database by filename

    \param target QObject to call on completion
    \param method with signature void method(bool,const TrackInfo &[,const QVariant &])
    \param fileName
    \param payload optional data to pass through to method
   */
   void getTrackInfo( QObject *target, const QString &method,
                      const QString &fileName,
                      const QVariant &payload = QVariant() );

   /*!
    \brief update track information to database

   */
   void updateTrackInfo( const TrackInfo &trackInfo, bool allowinsert = false );

   /*!
    \brief delete track information from database

    \param trackInfo
   */
   void deleteTrackInfo( const TrackInfo &trackInfo );

   /*!
    \brief delete track information from database by file name

    \param fileName
   */
   void deleteTrackInfo( const QString &fileName );

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

    \param folder
   */
   void insertFolder( const QString &folder );

   /*!
    \brief remove a folder

    \param folder
   */
   void deleteFolder( const QString &folder );

   /*!
    \brief rename

    \param oldName
    \param newName
   */
   void rename( const QString &oldName, const QString &newName );

   /*!
    \brief \todo complete documentation

   */
   enum Column { Artist, Title, Album, Genre };

   /*!
    \brief get all unique entries of a column

    \param target QObject to call on completion
    \param method with signature void method(const QStringList &[,const QVariant &])
    \param column
    \param payload optional data to pass through to method
   */
   void getAllColumnData( QObject *target, const QString &method,
                          Column column,
                          const QVariant &payload = QVariant() );

   /*!
    \brief generate heavy load on database by creating a table, filling it with a
      lot of data, dropping it and calling "VACUUM";

    \param target QObject to call on completion
    \param method with signature void method([const QVariant &])
    \param payload optional data to pass through to method
   */
   void generateTestLoad( QObject *target, const QString &method,
                          const QVariant &payload = QVariant() );

   /*!
    \brief for synchronization purposes

    \param target QObject to call
    \param method with signature void method([const QVariant &])
    \param payload optional data to pass through to method
   */
   void call( QObject *target, const QString &method,
              const QVariant &payload = QVariant() );

private:
   DatabaseInterface( const QString &fileName = QString() );
   virtual ~DatabaseInterface();

   Q_DISABLE_COPY( DatabaseInterface )

   static DatabaseInterface   *cpInterface;
   DatabaseThread             *mpDatabase;
};

/*! @} */

#endif
