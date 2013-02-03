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
class SorcererInterface;

/*!
  \addtogroup Underpass

  @{
  */


/*!
 \brief abstracting storage of station configuration data

*/
class StationStorage : public QObject
{
   Q_OBJECT

public:
   StationStorage( QObject *parent = 0 );
   virtual ~StationStorage();

   QStringList stationList() const;
   QStringList playerList() const;

   void createDefaults();
   void addStation( const QString &name, const QString &player, const QString &url );
   void removeStation( const QString &name );

public slots:
   void setStation( const QString &name );
   void setPlayer( const QString &text );
   void setUrl( const QString &text );
   void save();

signals:
   void url( const QString &text );
   void player( const QString &text );
   void stationListChanged();

private:
   Q_DISABLE_COPY( StationStorage )

   QSettings         *mpSettings; /*!< \brief instance of settings to write to */
   QTimer            *mpTimer; /*!< \brief timer to delay writing to settings */
   bool              mListChanged; /*!< \brief \todo TODO */
   const QString     mPrefix; /*!< \brief prefix ("subdirectory") of stations in settings */
   QString           mStation; /*!< \brief \todo TODO */
   QString           mUrl; /*!< \brief \todo TODO */
   QString           mPlayer; /*!< \brief \todo TODO */
};

/*! @} */

#endif /* STATIONSTORAGE_HPP */
