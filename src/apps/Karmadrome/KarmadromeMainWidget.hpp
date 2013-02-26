/*
 * src/apps/Karmadrome/KarmadromeMainWidget.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef KARMADROMEMAINWIDGET_HPP
#define KARMADROMEMAINWIDGET_HPP KARMADROMEMAINWIDGET_HPP

/* base class */
#include <QWidget>

/* system headers */

/* Qt headers */

/* local library headers */
#include <TrackInfoWidget.hpp>

/* local headers */

/* forward declaration of Qt classes */
class QAction;
class QIcon;
class QLineEdit;
class QMenu;
class QPushButton;
class QString;
class QTimer;

/* forward declaration of local classes */
class ButtonsWidget;
class KarmadromeConfigDialog;
class DatabaseInterface;
class GenericSatelliteHandler;
class Satellite;
class ScrollLine;

/*!
  \addtogroup Karmadrome

  \brief application: playlist manager

  Named after [the song by Pop Will Eat Itself](https://en.wikipedia.org/wiki/Pop_Will_Eat_Itself#Singles).

  @{
  */


/*!
 \brief

 \dotfile "graphs/apps/Karmadrome/KarmadromeMainWidget_connect.dot" "Connect Graph"
*/
class KarmadromeMainWidget : public QWidget
{
   Q_OBJECT

public:
   /*!
    \brief

    \param parent
    \param flags
   */
   KarmadromeMainWidget( QWidget *parent = 0, Qt::WindowFlags flags = 0 );
   /*!
    \brief

   */
   virtual ~KarmadromeMainWidget();

public slots:
   /*!
    \brief add currently played track to the list

    \param widget
   */

   void addToList( QWidget *widget );
   /*!
    \brief handle SLART message

    \param msg
   */

   void handleSatellite( const QByteArray &msg );
   /*!
    \brief handle adding of new playlist

   */

   void handleAdd();
   /*!
    \brief handle menu to export of playlist to m3u file

    \param action
   */

   void handleExport( QAction *action );
   /*!
    \brief handle menu to import of playlist from m3u file

    \param action
   */
   void handleImport( QAction *action );

   /*!
    \brief handle removal of playlist

    \param action
   */
   void handleRemove( QAction *action );

   /*!
    \brief handle "Read Clipboard" button

   */
   void handleReadButton();

   /*!
    \brief label "Read Clipboard" button according to settings

   */
   void labelReadButton();

   /*!
    \brief update the lists containing the playlists

   */
   void updateLists();

   /*!
    \brief slot for timer to send out k0u notification

   */
   void sendK0u();

   /*!
    \brief sync TrackInfo data

    \param trackInfo
   */
   void updateTrackInfo( const TrackInfo &trackInfo );

   /*!
    \brief \todo complete documentation

    \param enabled
   */
   void setButtonsEnabled( bool enabled = true );

   /*!
    \brief \todo complete documentation

    \param list
   */
   void updateGroupNames( const QStringList &list );

   /*!
    \brief \todo complete documentation

    \param entries
    \param payload folder parameter sent back from database
   */
   void removeGroup( const QStringList &entries, const QVariant &payload );

   /*!
    \brief \todo complete documentation

    \param trackInfo
    \param payload folder parameter sent back from database
   */
   void removeGroupFromTrack( const TrackInfo &trackInfo, const QVariant &payload );

private:
   Q_DISABLE_COPY( KarmadromeMainWidget )

   DatabaseInterface       *mpDatabase; /*!< \brief handle for running database jobs */
   Satellite               *mpSatellite; /*!< \brief handle for interprocess communication */
   GenericSatelliteHandler *mpGenericSatelliteHandler; /*!< \brief handling generic interprocess communication messages */
   ScrollLine              *mpFileName; /*!< \brief widget for displaying filename */
   TrackInfoWidget         *mpTrackInfo; /*!< \brief widget for displaying track information */
   QPushButton             *mpReadButton; /*!< \brief read button */
   QPushButton             *mpExportButton; /*!< \brief export button */
   QMenu                   *mpExportMenu; /*!< \brief drop down menu for export button */
   QAction                 *mpExportFavorite; /*!< \brief action for handling favorite tracks in export menu */
   QAction                 *mpExportUnwanted; /*!< \brief action for handling favorite tracks in export menu */
   QPushButton             *mpImportButton; /*!< \brief import button */
   QMenu                   *mpImportMenu; /*!< \brief drop down menu for import button */
   QAction                 *mpImportFavorite; /*!< \brief action for handling favorite tracks in import menu */
   QAction                 *mpImportUnwanted; /*!< \brief action for handling favorite tracks in import menu */
   ButtonsWidget           *mpListButtons; /*!< \brief widget for displaying folder buttons */

   QPushButton             *mpSettingsButton; /*!< \brief settings button */
   QPushButton             *mpAddButton; /*!< \brief add folder button */
   QPushButton             *mpRemoveButton; /*!< \brief remove folder button */
   QMenu                   *mpRemoveMenu; /*!< \brief drop down list for folder button */
   KarmadromeConfigDialog  *mpConfigDialog; /*!< \brief configuration dialog */
   QTimer                  *mpTimer; /*!< \brief timer for sending updates */

   QString                 mGroupNameToRemove; /*!< \brief  */
   QStringList             mGroups; /*!< \brief  */
   TrackInfo               mTrackInfo; /*!< \brief  */
};

/*! @} */

#endif
