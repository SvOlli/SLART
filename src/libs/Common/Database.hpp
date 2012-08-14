/*
 * src/libs/Common/Database.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef DATABASE_HPP
#define DATABASE_HPP DATABASE_HPP

/* base class */
#include <QDialog>

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
 \brief \todo complete documentation

*/
class Database
{
public:
   Database( const QString &fileName = QString() );
   virtual ~Database();

   /*!
    \brief test for the existance of the database file

   */
   static bool exists();

   /*!
    \brief start an encapsulated database transaction

   */
   bool beginTransaction();
   /*!
    \brief end an encapsulated database transaction

   */
   bool endTransaction( bool commit );
   /*!
    \brief 

   */
   void registerUpdate( Satellite *satellite, const QByteArray &message );
   /*!
    \brief disable the next satellite notify

   */
   void disableNotify();
   /*!
    \brief try to clean up and compress database

   */
   void cleanup();

   /*!
    \brief get track information from database

    \param trackInfo (trackInfo = 0: only test if fileName is in database)
    \param fileName (empty fileName: search for TrackInfo by trackInfo->mID)
   */
   bool getTrackInfo( TrackInfo *trackInfo, const QString &fileName = QString() );
   /*!
    \brief update track information to database

   */
   void updateTrackInfo( const TrackInfo *trackInfo, bool allowinsert = false );
   /*!
    \brief delete track information from database

   */
   void deleteTrackInfo( const TrackInfo *trackInfo );
   /*!
    \brief get a list of track information matching to the search string

   */
   int getTrackInfoList( TrackInfoList *trackInfoList, const QString &search = QString() );
   /*!
    \brief get a list of file names matching to the search string

   */
   int getPathNameList( QStringList *pathNameList, const QString &search = QString() );
   /*!
    \brief get a random track

   */
   bool getRandomTrack( TrackInfo *trackInfo, bool favorite,
                        bool leastplayed,
                        const QStringList &excludeArtists,
                        const QString &folder = QString() );

   /*!
    \brief get a list of all available folders

   */
   QStringList getFolders();
   /* get all entries of a list
      QChar(1) = Favorite, QChar(2) = No Auto */
   QStringList getFolder( const QString &folder );
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
   void rename( const QString &newName,
                const QString &oldDirName, const QString &oldFileName = QString() );
   /*!
    \brief 

   */
   QStringList getAllColumnData( const QString &columnName );

private:
   Database( const Database &that );
   Database &operator=( const Database &that );

   /*!
    \brief send out an error description to Innuendo

   */
   void logError( const QString &note = QString() );

   /*!
    \brief generate the filename for database

   */
   static QString getDatabaseFileName();

   QSqlDatabase       *mpSqlDB;
   QSqlQuery          *mpQuery;
   Satellite          *mpSatellite;
   unsigned int       mDatabaseVersion;
   const unsigned int mCodeVersion;
   QByteArray         mUpdateMessage;
   bool               mNotifyDisabled;
};

/*! @} */

#endif
