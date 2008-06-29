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
   void shutdown();

   bool open();
   bool beginTransaction() { return mpSqlDB->transaction(); };
   bool endTransaction( bool commit ) { return commit ? mpSqlDB->commit() : mpSqlDB->rollback(); };
   
   bool getTrackInfoByFileName( TrackInfo *trackInfo, const QString &fileName );
   void updateTrackInfo( const TrackInfo *trackInfo );
   void deleteTrackInfo( const TrackInfo *trackInfo );
   
   unsigned int getTrackInfoList( TrackInfoList *trackInfoList );
   
   QStringList getFolders();
   void insertFolder( const QString &folder );
   void deleteFolder( const QString &folder );
   
public slots:

signals:
   
private:
   Database( const Database &other );
   Database &operator=( const Database &other );

   QSqlDatabase *mpSqlDB;
   QSqlQuery    *mpQuery;
   unsigned int mVersion;
};

#endif
