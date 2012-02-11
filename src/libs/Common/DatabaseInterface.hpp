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
#include <QStringList>

/* local library headers */

/* local headers */
#include "TrackInfo.hpp"

/* forward declaration of Qt classes */
class QSqlDatabase;
class QSqlQuery;

/* forward declaration of local classes */
class DatabaseThread;
class Satellite;


/*!
  \addtogroup Common

  @{
*/

/*!
 \brief TODO

 \dotfile "graphs/libs/Common/DatabaseInterface_connect.dot" "Connect Graph"
*/
class DatabaseInterface
{
public:
   /*!
    \brief 

   */
   static DatabaseInterface *create();
   /*!
    \brief 

   */
   static void destroy();
   /*!
    \brief 

   */
   static DatabaseInterface *get();

   /*!
    \brief test for the existance of the Database file

   */
   static bool exists();

   /*!
    \brief 

   */
   void connectActivityIndicator( QObject *object, const char *slot );

   /*!
    \brief 

   */
   void disconnectActivityIndicator( QObject *object, const char *slot );

   /*!
    \brief 

   */
   void registerUpdate( Satellite *satellite, const QByteArray &message );

   /*!
    \brief disable the next satellite notify

   */
   void disableNotify();

   /*!
    \brief get track information from database by id
      return call parameters: bool found, TrackInfo

    \param target
    \param method
    \param id
   */
   void getTrackInfo( QObject *target, const QString &method,
                      int id );

   /*!
    \brief get track information from database by filename
      return call parameters: bool found, TrackInfo

    \param target
    \param method
    \param fileName
   */
   void getTrackInfo( QObject *target, const QString &method,
                      const QString &fileName );

   /*!
    \brief update track information to database

   */
   void updateTrackInfo( const TrackInfo &trackInfo, bool allowinsert = false );

   /*!
    \brief delete track information from database

   */
   void deleteTrackInfo( const TrackInfo &trackInfo );

   /*!
    \brief delete track information from database by file name

   */
   void deleteTrackInfo( const QString &fileName );

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
    \brief TODO

   */
   enum Column { Artist, Title, Album, Genre };

   /*!
    \brief get all unique entries of a column
      return call parameters: QStringList

    \param target
    \param method
    \param column
   */
   void getAllColumnData( QObject *target, const QString &method,
                          Column column );

   /*!
    \brief for synchronization purposes

   */
   void call( QObject *target, const QString &method );

private:
   DatabaseInterface( const QString &fileName = QString() );
   virtual ~DatabaseInterface();

   Q_DISABLE_COPY( DatabaseInterface )

   static DatabaseInterface   *cpInterface;
   DatabaseThread             *mpDatabase;
};

/*! @} */

#endif
