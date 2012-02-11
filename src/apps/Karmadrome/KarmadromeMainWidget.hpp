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
class ConfigDialog;
class DatabaseInterface;
class GenericSatMsgHandler;
class Satellite;
class ScrollLine;

/*!
  \addtogroup Karmadrome Karmadrome: playlist manager
  @{
  */


/*!
 \brief

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
    \brief TODO

    \param enabled
   */
   void setButtonsEnabled( bool enabled = true );

   /*!
    \brief TODO

    \param list
   */
   void updateFolderNames( const QStringList &list );

   /*!
    \brief TODO

    \param entries
   */
   void removeFolder( const QStringList &entries );

   /*!
    \brief TODO

    \param trackInfo
   */
   void removeFolderFromTrack( const TrackInfo &trackInfo );

signals:
   /*!
    \brief TODO

    \param icon
    \param title
   */
   void requestChangeTitle( const QIcon &icon, const QString &title );

private:
   Q_DISABLE_COPY( KarmadromeMainWidget )

   DatabaseInterface    *mpDatabase; /*!< TODO */
   Satellite            *mpSatellite; /*!< TODO */
   GenericSatMsgHandler *mpGenericSatMsgHandler; /*!< TODO */
   ScrollLine           *mpFileName; /*!< TODO */
   TrackInfoWidget      *mpTrackInfo; /*!< TODO */
   QPushButton          *mpReadButton; /*!< TODO */
   QPushButton          *mpExportButton; /*!< TODO */
   QMenu                *mpExportMenu; /*!< TODO */
   QAction              *mpExportFavorite; /*!< TODO */
   QAction              *mpExportUnwanted; /*!< TODO */
   QPushButton          *mpImportButton; /*!< TODO */
   QMenu                *mpImportMenu; /*!< TODO */
   QAction              *mpImportFavorite; /*!< TODO */
   QAction              *mpImportUnwanted; /*!< TODO */
   ButtonsWidget        *mpListButtons; /*!< TODO */

   QPushButton          *mpSettingsButton; /*!< TODO */
   QPushButton          *mpAddButton; /*!< TODO */
   QPushButton          *mpRemoveButton; /*!< TODO */
   QMenu                *mpRemoveMenu; /*!< TODO */
   ConfigDialog         *mpConfigDialog; /*!< TODO */
   QTimer               *mpTimer; /*!< TODO */
   QAction              *mpCurrentAction; /*!< TODO */

   QStringList          mFolders; /*!< TODO */
   TrackInfo            mTrackInfo; /*!< TODO */
};

/*! @} */

#endif
