/**
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
   /* add currently played track to the list */
   /*!
    \brief

    \param widget
   */
   void addToList( QWidget *widget );
   /* handle SLART message */
   /*!
    \brief

    \param msg
   */
   void handleSatellite( const QByteArray &msg );
   /* handle adding of new playlist */
   /*!
    \brief

   */
   void handleAdd();
   /* handle menu to export of playlist to m3u file */
   /*!
    \brief

    \param action
   */
   void handleExport( QAction *action );
   /* handle menu to import of playlist from m3u file */
   /*!
    \brief

    \param action
   */
   void handleImport( QAction *action );
   /* handle removal of playlist */
   /*!
    \brief

    \param action
   */
   void handleRemove( QAction *action );
   /* handle "Read Clipboard" button */
   /*!
    \brief

   */
   void handleReadButton();
   /* label "Read Clipboard" button according to settings */
   /*!
    \brief

   */
   void labelReadButton();
   /* update the lists containing the playlists */
   /*!
    \brief

   */
   void updateLists();
   /* slot for timer to send out k0u notification */
   /*!
    \brief

   */
   void sendK0u();
   /* sync TrackInfo data */
   /*!
    \brief

    \param trackInfo
   */
   void updateTrackInfo( const TrackInfo &trackInfo );
   /*  */
   /*!
    \brief

    \param enabled
   */
   void setButtonsEnabled( bool enabled = true );
   /*  */
   /*!
    \brief

    \param list
   */
   void updateFolderNames( const QStringList &list );
   /*  */
   /*!
    \brief

    \param entries
   */
   void removeFolder( const QStringList &entries );
   /*  */
   /*!
    \brief

    \param trackInfo
   */
   void removeFolderFromTrack( const TrackInfo &trackInfo );

signals:
   /*!
    \brief

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
