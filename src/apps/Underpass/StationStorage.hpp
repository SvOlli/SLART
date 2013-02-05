/*
 * src/libs/Underpass/StationStorage.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef STATIONSTORAGE_HPP
#define STATIONSTORAGE_HPP STATIONSTORAGE_HPP

/* base class */
#include <QObject>

/* system headers */

/* Qt headers */
#include <QStringList>

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */
class QSettings;
class QTimer;

/* forward declaration of local classes */


/*!
  \addtogroup Underpass

  @{
*/

/*!
 \brief abstracting storage of station configuration data

 \dotfile "graphs/apps/Underpass/StationStorage_connect.dot" "Connect Graph"
*/
class StationStorage : public QObject
{
   Q_OBJECT

public:
   /*!
    \brief constructor

    \param parent
   */
   StationStorage( QObject *parent = 0 );
   /*!
    \brief destructor

   */
   virtual ~StationStorage();

   /*!
    \brief get a list of all stations stored

    \return QStringList
   */
   QStringList stationList() const;
   /*!
    \brief get a list of all player (configurations) stored

    \return QStringList
   */
   QStringList playerList() const;

   /*!
    \brief create default stations

   */
   void createDefaults();
   /*!
    \brief add a station

    \param name informal name to identify the station
    \param player player used for playing
    \param url url of the station
   */
   void addStation( const QString &name, const QString &player, const QString &url );
   /*!
    \brief remove a station

    \param name name of station to remove
   */
   void removeStation( const QString &name );

   /*!
    \brief name of station
   */
   Q_PROPERTY( QString station
               READ station
               WRITE setStation )
   /*!
    \brief return name of station

    \return QString
   */
   QString station() const;

   /*!
    \brief url of station
   */
   Q_PROPERTY( QString url
               READ url
               WRITE setUrl )
   /*!
    \brief return url of station

    \return QString
   */
   QString url() const;

   /*!
    \brief player used for station
   */
   Q_PROPERTY( QString player
               READ player
               WRITE setPlayer )
   /*!
    \brief return player identification string

    \return QString
   */
   QString player() const;

   /*!
    \brief has station list changed? (will be reset after read)
   */
   Q_PROPERTY( bool stationListChanged
               READ stationListChanged )
   /*!
    \brief read if station list changed (will reset after read)

    \return bool
   */
   bool stationListChanged();

public slots:
   /*!
    \brief set the currently used station

    \param name
   */
   void setStation( const QString &name );
   /*!
    \brief set the currently used player

    \param text
   */
   void setPlayer( const QString &text );
   /*!
    \brief set the url to use

    \param text
   */
   void setUrl( const QString &text );
   /*!
    \brief internal method save the configuration

    This method if called by mpTimer, so not all typing changes will be written
    to the settings.
   */
   void save();

private:
   Q_DISABLE_COPY( StationStorage )

   QSettings         *mpSettings; /*!< \brief instance of settings to write to */
   QTimer            *mpTimer; /*!< \brief timer to delay writing to settings */
   bool              mListChanged; /*!< \brief marker if stationListChanged needs to be emitted */
   const QString     mPrefix; /*!< \brief prefix ("subdirectory") of stations in settings */
   QString           mStation; /*!< \brief name of current \ref station */
   QString           mUrl; /*!< \brief \ref url of current station */
   QString           mPlayer; /*!< \brief \ref player of current station */
};

/*! @} */

#endif /* STATIONSTORAGE_HPP */
