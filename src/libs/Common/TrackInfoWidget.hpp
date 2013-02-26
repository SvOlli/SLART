/*
 * src/libs/Common/TrackInfoWidget.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef TRACKINFOWIDGET_HPP
#define TRACKINFOWIDGET_HPP TRACKINFOWIDGET_HPP

/* base class */
#include <QWidget>

/* system headers */

/* Qt headers */
#include <QString>

/* local library headers */

/* local headers */
#include "TrackInfo.hpp"

/* forward declaration of Qt classes */
class QCheckBox;
class QLabel;
class QPushButton;

/* forward declaration of local classes */
class Database;
class ImageWidget;
class ScrollLine;


/*!
  \addtogroup Common

  @{
*/

/*!
 \brief \todo complete documentation

 \dotfile "graphs/libs/Common/TrackInfoWidget_connect.dot" "Connect Graph"
*/
class TrackInfoWidget : public QWidget
{
   Q_OBJECT

public:
   /*!
    \brief \todo

    \param includeGroups
    \param parent
   */
   TrackInfoWidget( bool includeGroups, QWidget *parent = 0 );

   /*!
    \brief \todo

    \deprecated only used by Partyman

    \param database
    \param includeGroups
    \param parent
   */
   TrackInfoWidget( Database *database,
                    bool includeGroups, QWidget *parent = 0 );
   /*!
    \brief \todo

   */
   virtual ~TrackInfoWidget();

public slots:
   /*!
    \brief handle the "Favorite" checkbox

   */
   void handleFavoriteButton();
   /*!
    \brief handle the "No Auto" checkbox

   */
   void handleUnwantedButton();
   /*!
    \brief display a menu to change groups part 1: get groups from database

   */
   void handleGroupsMenu();
   /*!
    \brief display a menu to change groups part 2: get selection an update

    \param groups
   */
   void handleGroupsMenu( const QStringList &groups );
   /*!
    \brief remotely set both "Favorite" and "No Auto" checkboxes

    \param favorite
    \param unwanted
   */
   void setFavoriteUnwanted( bool favorite, bool unwanted );
   /*!
    \brief update the track data

    \param trackInfo
   */
   void getTrack( const TrackInfo &trackInfo );
   /*!
    \brief update the displayed track info

    \param reread
   */
   void update( bool reread = true );
   /*!
    \brief 

    \param trackInfo
   */
   void updateTrackInfo( const TrackInfo &trackInfo );
   /*!
    \brief \todo

    \param coverImage
   */
   void setCoverImage( const QImage &coverImage );

signals:
   /*!
    \brief either "Favorite" or "No Auto" checkbox has been clicked

    \param trackInfo
   */
   void checkboxClicked( const TrackInfo &trackInfo );

private:
   Q_DISABLE_COPY( TrackInfoWidget )

   Database    *mpDatabase; /*!< \brief \todo TODO */
   TrackInfo   mTrackInfo; /*!< \brief \todo TODO */
   bool        mIncludeGroups; /*!< \brief \todo TODO */
   QLabel      *mpTimesPlayed; /*!< \brief \todo TODO */
   ScrollLine  *mpArtist; /*!< \brief \todo TODO */
   ScrollLine  *mpTitle; /*!< \brief \todo TODO */
   ScrollLine  *mpAlbum; /*!< \brief \todo TODO */
   ScrollLine  *mpTrackNr; /*!< \brief \todo TODO */
   QCheckBox   *mpFavoriteButton; /*!< \brief \todo TODO */
   QCheckBox   *mpUnwantedButton; /*!< \brief \todo TODO */
   ImageWidget *mpCoverArt; /*!< \brief \todo TODO */
};

/*! @} */

#endif
