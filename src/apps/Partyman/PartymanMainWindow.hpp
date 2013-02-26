/*
 * src/apps/Partyman/PartymanMainWindow.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef PARTYMANMAINWINDOW_HPP
#define PARTYMANMAINWINDOW_HPP PARTYMANMAINWINDOW_HPP

/* base class */
#include <QMainWindow>

/* system headers */

/* Qt headers */

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */
class QDockWidget;
class QModelIndex;
class QPushButton;
class QTextBrowser;

/* forward declaration of local classes */
class ControlWidget;
class Database;
class FileSysTreeModel;
class FileSysTreeUpdate;
class FileSysTreeView;
class PartymanConfigDialog;
class PlaylistControlWidget;
class PlaylistContentWidget;
class SearchTrackWidget;
class TrackInfo;
class TrackInfoWidget;

/*!
  \addtogroup Partyman

  \brief application: dual slot audio player

  Named after [the song by Prince](https://en.wikipedia.org/wiki/Partyman).

  @{
  */

/*!
 \brief the central widget

 \dotfile "graphs/apps/Partyman/PartymanMainWindow_connect.dot" "Connect Graph"
*/
class PartymanMainWindow : public QMainWindow
{
   Q_OBJECT

public:
   /*!
    \brief constuctor

    \param parent
    \param flags
   */
   PartymanMainWindow( QWidget *parent = 0, Qt::WindowFlags flags = 0 );

   /*!
    \brief destructor

   */
   virtual ~PartymanMainWindow();

   /*!
    \brief determine the filename of the next track

    \param fileName pointer to write filename to
   */

   void getNextTrack( QString *fileName );
   /*!
    \brief set "favorite" and "unwanted" flags for current track

    \param favorite
    \param unwanted
   */

   void setTrackInfoFavoriteUnwanted( bool favorite, bool unwanted );
   /*!
    \brief set the tracks for the two players

    \param current
    \param next
   */
   void setCurrentNext( const QString &current = QString(),
                        const QString &next = QString() );

   /*!
    \brief set the data for the track info widget

    \param trackInfo
   */
   void setTrackInfo( const TrackInfo &trackInfo );

   /*!
    \brief set icon and/or title of all main windows (usually just one)

    \param icon icon to set (if not to be set, pass null QIcon())
    \param title icon to set (if not to be set, pass null QString())
   */
   static void setIconAndTitle( const QIcon &icon, const QString &title );

public slots:
   /*!
    \brief handle request for new icon and title

    \param icon
    \param title
   */
   void changeTitle( const QIcon &icon, const QString &title );

   /*!
    \brief prohibit closing of window (kiosk mode)

    \param prohibit
   */
   void prohibitClose( bool prohibit );

   /*!
    \brief add entries to playlist

    \param qmi
   */
   void addEntries( const QModelIndex &qmi );

   /*!
    \brief remove track from playlist

    \param qmi
    \param key
   */
   void deleteEntry( const QModelIndex &qmi, int key );

   /*!
    \brief add entries to playlist

    \param entries
    \param atStart insert them at start instead of at the end
   */
   void addEntries( const QStringList &entries, bool atStart = false );

   /*!
    \brief read the configuration

   */
   void readConfig();

   /*!
    \brief update the browse tab by reading from database

    \param initial initial run on startup?
   */
   void startBrowserUpdate( bool initial = false );

   /*!
    \brief handler for completion of startBrowserUpdate

   */
   void finishBrowserUpdate();

   /*!
    \brief update the track info data structure

   */
   void updateTrackInfo();

   /*!
    \brief save the current playlist to registry

   */
   void savePlaylist();

   /*!
    \brief raise a tab by name

    \param name
   */
   void showTab( const QString &name );

   /*!
    \brief \todo complete documentation

    \param bool
   */
   void setDockFocus( bool );

signals:
   /*!
    \brief \todo complete documentation

    \param qmi
   */
   void expand( const QModelIndex &qmi );

   /*!
    \brief \todo complete documentation

    \param isValid
   */
   void playlistIsValid( bool isValid );

protected:
   /*!
    \brief intercept for writing the settings

    \param event
   */
   virtual void closeEvent( QCloseEvent *event );

   /*!
    \brief very ugly workaround for wrong position restoration on Ubuntu

    \param event
   */
   virtual bool event( QEvent *event );

private:
   Q_DISABLE_COPY( PartymanMainWindow )

   /*!
    \brief add entries to a stringlist

   */
   void addEntries( QStringList *list, const QModelIndex &qmi );

   /*!
    \brief get a random track from database

   */
   bool getRandomTrack( QString *fileName, QStringList *playedArtists, int randomTries,
                        bool favoriteOnly, bool leastPlayed, const QString &playGroup );

   /*!
    \brief \todo complete documentation

    \param widget
    \param name
    \param label
    \param shortCut
    \return QDockWidget
   */
   QDockWidget *setupDock( QWidget *widget, const QString &name,
                           const QString &label, const QKeySequence &shortCutKey );

   bool                    mProhibitCloseWindow; /*!< \brief flag to store of close is prohibited */
   int                     mForbidMove; /*!< \brief \todo TODO */
   Database                *mpDatabase; /*!< \brief database instance */
   PartymanConfigDialog    *mpConfig; /*!< \brief configuration dialog */
   PlaylistContentWidget   *mpPlaylistContent; /*!< \brief playlist widget */
   ControlWidget           *mpControl; /*!< \brief control widget */
   QTextBrowser            *mpHelpText; /*!< \brief help widget */
   TrackInfoWidget         *mpTrackInfo; /*!< \brief track info widget */
   SearchTrackWidget       *mpSearch; /*!< \brief search widget */
   FileSysTreeView         *mpTreeView; /*!< \brief database tree view */
   FileSysTreeModel        *mpTreeModel; /*!< \brief database tree model */
   FileSysTreeModel        *mpNextTreeModel; /*!< \brief database tree model used for update */
   FileSysTreeUpdate       *mpTreeUpdate; /*!< \brief database tree update helper */
   QDockWidget             *mpDockTreeView; /*!< \brief dock containing database tree */
   QString                 mCurrentFile; /*!< \brief helper to store current track */
   QString                 mNextFile; /*!< \brief helper to store upcoming track */

   static PartymanMainWindow  *cpMainWindow; /*!< \brief handle for setIconAndTitle() */
};

/*! @} */

#endif
