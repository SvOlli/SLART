/**
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


class DatabaseInterface
{
public:
   static DatabaseInterface *get();
   static void destroy();

   /* test for the existance of the Database file */
   static bool exists();

   /*  */
   void connectActivityIndicator( QObject *object, const char *slot );

   /*  */
   void disconnectActivityIndicator( QObject *object, const char *slot );

   /*  */
   void registerUpdate( Satellite *satellite, const QByteArray &message );

   /* disable the next satellite notify */
   void disableNotify();

   /* get track information from database by id
      return call parameters: bool found, TrackInfo */
   void getTrackInfo( QObject *target, const QString &method,
                      int id );

   /* get track information from database by filename
      return call parameters: bool found, TrackInfo */
   void getTrackInfo( QObject *target, const QString &method,
                      const QString &fileName );

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
   void rename( const QString &newName,
                const QString &oldDirName, const QString &oldFileName = QString() );

   enum Column { Artist, Title, Album, Genre };
   /* get all unique entries of a column
      return call parameters: QStringList */
   void getAllColumnData( QObject *target, const QString &method,
                          Column column );

private:
   DatabaseInterface( const QString &fileName = QString() );
   virtual ~DatabaseInterface();

   DatabaseInterface( const DatabaseInterface &that );
   DatabaseInterface &operator=( const DatabaseInterface &that );

   static DatabaseInterface   *cpInterface;
   DatabaseThread             *mpDatabase;
};

#endif
