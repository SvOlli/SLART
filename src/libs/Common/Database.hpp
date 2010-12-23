/**
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
class GlobalConfigWidget;
class Satellite;


class Database
{
public:
   Database( const QString &fileName = QString() );
   virtual ~Database();

   /* test for the existance of the database file */
   static bool exists();

   /* start an encapsulated database transaction */
   bool beginTransaction();
   /* end an encapsulated database transaction */
   bool endTransaction( bool commit );
   /*  */
   void registerUpdate( Satellite *satellite, const QByteArray &message );
   /* disable the next satellite notify */
   void disableNotify();
   /* try to clean up and compress database */
   void cleanup();

   /* get track information from database
      empty fileName: search for TrackInfo by trackInfo->mID
      trackInfo = 0: only test if fileName is in database */
   bool getTrackInfo( TrackInfo *trackInfo, const QString &fileName = QString() );
   /* update track information to database */
   void updateTrackInfo( const TrackInfo *trackInfo, bool allowinsert = false );
   /* delete track information from database */
   void deleteTrackInfo( const TrackInfo *trackInfo );
   /* get a list of track information matching to the search string */
   int getTrackInfoList( TrackInfoList *trackInfoList, const QString &search = QString() );
   /* get a list of file names matching to the search string */
   int getPathNameList( QStringList *pathNameList, const QString &search = QString() );
   /* get a random track */
   bool getRandomTrack( TrackInfo *trackInfo, bool favorite,
                        bool leastplayed, const QString &folder = QString() );

   /* get a list of all available folders */
   QStringList getFolders();
   /* get all entries of a list
      QChar(1) = Favorite, QChar(2) = No Auto */
   QStringList getFolder( const QString &folder );
   /* add a folder */
   void insertFolder( const QString &folder );
   /* remove a folder */
   void deleteFolder( const QString &folder );
   /* rename */
   void rename( const QString &newName,
                const QString &oldDirName, const QString &oldFileName = QString() );
   /*  */
   QStringList getAllColumnData( const QString &columnName );

private:
   Database( const Database &that );
   Database &operator=( const Database &that );

   /* send out an error description to Innuendo */
   void logError( const QString &note = QString() );

   /* generate the filename for database */
   static QString getDatabaseFileName();

   QSqlDatabase       *mpSqlDB;
   QSqlQuery          *mpQuery;
   Satellite          *mpSatellite;
   unsigned int       mDatabaseVersion;
   const unsigned int mCodeVersion;
   QByteArray         mUpdateMessage;
   bool               mNotifyDisabled;
};

#endif
