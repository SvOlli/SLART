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
class ConfigDialog;
class ControlWidget;
class Database;
class FileSysTreeModel;
class FileSysTreeUpdate;
class FileSysTreeView;
class PlaylistControlWidget;
class PlaylistContentWidget;
class SearchWidget;
class TrackInfo;
class TrackInfoWidget;

/*!
  \addtogroup Partyman Partyman: dual slot audio player
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

signals:
   void expand( const QModelIndex &qmi );
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
                        bool favoriteOnly, bool leastPlayed, const QString &playFolder );

   bool                    mProhibitCloseWindow; /*!< \brief flag to store of close is prohibited */
   int                     mForbidMove;
   Database                *mpDatabase; /*!< \todo */
   ConfigDialog            *mpConfig; /*!< \todo */
   PlaylistContentWidget   *mpPlaylistContent; /*!< \todo */
   ControlWidget           *mpControl; /*!< \todo */
   QTextBrowser            *mpHelpText; /*!< \brief \todo TODO */
   TrackInfoWidget         *mpTrackInfo; /*!< \brief \todo TODO */
   SearchWidget            *mpSearch; /*!< \brief \todo TODO */
   FileSysTreeView         *mpTreeView; /*!< \brief \todo TODO */
   FileSysTreeModel        *mpTreeModel; /*!< \brief \todo TODO */
   FileSysTreeModel        *mpNextTreeModel; /*!< \brief \todo TODO */
   FileSysTreeUpdate       *mpTreeUpdate; /*!< \brief \todo TODO */
   QDockWidget             *mpDockTreeView;
   QString                 mCurrentFile;
   QString                 mNextFile;
};

/*! @} */

#endif
