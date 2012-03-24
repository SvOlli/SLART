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
   void updateFolderNames( const QStringList &list );

   /*!
    \brief \todo complete documentation

    \param entries
   */
   void removeFolder( const QStringList &entries );

   /*!
    \brief \todo complete documentation

    \param trackInfo
   */
   void removeFolderFromTrack( const TrackInfo &trackInfo );

signals:
   /*!
    \brief \todo complete documentation

    \param icon
    \param title
   */
   void requestChangeTitle( const QIcon &icon, const QString &title );

private:
   Q_DISABLE_COPY( KarmadromeMainWidget )

   DatabaseInterface    *mpDatabase; /*!< \todo */
   Satellite            *mpSatellite; /*!< \todo */
   GenericSatMsgHandler *mpGenericSatMsgHandler; /*!< \todo */
   ScrollLine           *mpFileName; /*!< \todo */
   TrackInfoWidget      *mpTrackInfo; /*!< \todo */
   QPushButton          *mpReadButton; /*!< \todo */
   QPushButton          *mpExportButton; /*!< \todo */
   QMenu                *mpExportMenu; /*!< \todo */
   QAction              *mpExportFavorite; /*!< \todo */
   QAction              *mpExportUnwanted; /*!< \todo */
   QPushButton          *mpImportButton; /*!< \todo */
   QMenu                *mpImportMenu; /*!< \todo */
   QAction              *mpImportFavorite; /*!< \todo */
   QAction              *mpImportUnwanted; /*!< \todo */
   ButtonsWidget        *mpListButtons; /*!< \todo */

   QPushButton          *mpSettingsButton; /*!< \todo */
   QPushButton          *mpAddButton; /*!< \todo */
   QPushButton          *mpRemoveButton; /*!< \todo */
   QMenu                *mpRemoveMenu; /*!< \todo */
   ConfigDialog         *mpConfigDialog; /*!< \todo */
   QTimer               *mpTimer; /*!< \todo */
   QAction              *mpCurrentAction; /*!< \todo */

   QStringList          mFolders; /*!< \todo */
   TrackInfo            mTrackInfo; /*!< \todo */
};

/*! @} */

#endif
