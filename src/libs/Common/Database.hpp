/**
 * src/libs/Common/Database.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef DATABASE_HPP
#define DATABASE_HPP DATABASE_HPP

#include <QDir>
#include <QFileInfo>
#include <QStringList>
#ifdef QT_SQL_LIB
#include <QtSql>
#else
class QSqlDatabase;
class QSqlQuery;
#endif

#include "TrackInfo.hpp"


class Database
{
public:
   Database( const QString &fileName = QString() );
   virtual ~Database();

   /* test for the existance of the database file */
   static bool exists() 
   {
      return QFileInfo( getDatabaseFileName() ).isFile();
   };

   /* start an encapsulated database transaction */
   bool beginTransaction();
   /* end an encapsulated database transaction */
   bool endTransaction( bool commit );
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
   
private:
   Database( const Database &other );
   Database &operator=( const Database &other );
   
   /* send out an error description to Innuendo */
   void logError( const QString &note = QString() );
   
   /* generate the filename for database */
   static QString getDatabaseFileName()
   {
#ifdef _WIN32
      QString slartdb( "/slart.db" );
#else
      QString slartdb( "/.slartdb" );
#endif
      return QDir::homePath() + slartdb;
   };

   QSqlDatabase       *mpSqlDB;
   QSqlQuery          *mpQuery;
   unsigned int       mDatabaseVersion;
   const unsigned int mCodeVersion;
};

#endif