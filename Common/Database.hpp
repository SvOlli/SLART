/**
 * Database.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#ifndef DATABASE_HPP
#define DATABASE_HPP DATABASE_HPP

#include <QStringList>
#include <QtSql>

#include "TrackInfo.hpp"

class Database : public QObject
{
Q_OBJECT

public:
   Database( const QString &fileName = QString() );
   virtual ~Database();

   /* generate the filename for database */
   static char *getDatabaseFileName();

   /* open the database file */
   bool open();
   /* start an encapsulated database transaction */
   bool beginTransaction() { return mpSqlDB->transaction(); };
   /* end an encapsulated database transaction */
   bool endTransaction( bool commit ) { return commit ? mpSqlDB->commit() : mpSqlDB->rollback(); };
   
   /* get track information from database */
   bool getTrackInfo( TrackInfo *trackInfo, const QString &fileName = QString() );
   /* update track information to database */
   void updateTrackInfo( const TrackInfo *trackInfo );
   /* delete track information from database */
   void deleteTrackInfo( const TrackInfo *trackInfo );
   /* get a list of track information matching to the search string */
   unsigned int getTrackInfoList( TrackInfoList *trackInfoList, const QString &search = QString() );
   /* get a random track */
   bool getRandomTrack( TrackInfo *trackInfo, bool favorite,
                        bool unplayed, const QString &folder = QString() );
   
   /* get a list of all available folders */
   QStringList getFolders();
   /* add a folder */
   void insertFolder( const QString &folder );
   /* remove a folder */
   void deleteFolder( const QString &folder );
   
public slots:

signals:
   
private:
   Database( const Database &other );
   Database &operator=( const Database &other );
   
   /* send out an error description to Innuendo */
   void logError( const QString &note = QString() );
   
   QSqlDatabase       *mpSqlDB;
   QSqlQuery          *mpQuery;
   unsigned int       mDatabaseVersion;
   const unsigned int mCodeVersion;
};

#endif
