/*
 * src/apps/Partyman/PlaylistControlWidget.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef PLAYLISTCONTROLWIDGET_HPP
#define PLAYLISTCONTROLWIDGET_HPP PLAYLISTCONTROLWIDGET_HPP

/* base class */
#include <QWidget>

/* system headers */

/* Qt headers */
#include <QStringList>

/* local library headers */

/* local headers */
#include "FileSysTreeView.hpp"
#include "FileSysTreeUpdate.hpp"

/* forward declaration of Qt classes */
class QLabel;
class QSignalMapper;
class QSplitter;
class QTabWidget;
class QTextBrowser;

/* forward declaration of local classes */
class ConfigDialog;
class Database;
class FileSysTreeModel;
class PlaylistContentWidget;
class SearchWidget;
class TrackInfo;
class TrackInfoWidget;


/*!
  \addtogroup Partyman

  @{
*/

/*!
 \brief \todo complete documentation

 \dotfile "graphs/apps/Partyman/PlaylistControlWidget_connect.dot" "Connect Graph"
*/
class PlaylistControlWidget : public QWidget
{
   Q_OBJECT

public:
   PlaylistControlWidget( Database *database, ConfigDialog *config,
                   QWidget *parent = 0, Qt::WindowFlags f = 0 );
   virtual ~PlaylistControlWidget();

   /*!
    \brief get name of next track either from playlist

   */
   void getNextTrack( QString *fileName );
   /*!
    \brief set checkboxes of trackinfo

   */
   void setTrackInfoFavoriteUnwanted( bool favorite, bool unwanted );
   /*!
    \brief save the current and next track for later saving of complete playlist

   */
   void setCurrentNext( const QString &current = QString(),
                        const QString &next = QString() );

public slots:
   /*!
    \brief add internal entries

   */
   void addEntries( const QModelIndex &qmi );
   /*!
    \brief delete entries

   */
   void deleteEntry( const QModelIndex &qmi, int key );
   /*!
    \brief add external entries

   */
   void addEntries( const QStringList &entries, bool atStart = false );
   /*!
    \brief save current tab for restart

   */
   void handleTabChange( int tabNr );
   /*!
    \brief re-read config parameters

   */
   void readConfig();
   /*!
    \brief pass the track info through to track info

   */
   void getTrack( const TrackInfo &trackInfo );
   /*!
    \brief start the background update of the browser

   */
   void startBrowserUpdate();
   /*!
    \brief finish up the browser background update

   */
   void finishBrowserUpdate();
   /*!
    \brief pass the update through to track info

   */
   void updateTrackInfo();
   /*!
    \brief save the playlist to settings (prepend current and next, if any)

   */
   void savePlaylist();

signals:
   /*!
    \brief unfold a node

   */
   void expand( const QModelIndex &qmi );
   /*!
    \brief tell if the current playlist is valid

   */
   void playlistIsValid( bool isValid );

private:
   PlaylistControlWidget( const PlaylistControlWidget &that );
   PlaylistControlWidget &operator=( const PlaylistControlWidget &that );

   /*!
    \brief add entries to a stringlist

   */
   void addEntries( QStringList *list, const QModelIndex &qmi );

   /*!
    \brief get a random track from database

   */
   bool getRandomTrack( QString *fileName, QStringList *playedArtists, int randomTries,
                        bool favoriteOnly, bool leastPlayed, const QString &playFolder );

   Database                *mpDatabase;
   ConfigDialog            *mpConfig;
   QTabWidget              *mpTabs;
   PlaylistContentWidget   *mpPlaylistContent;
   FileSysTreeView         *mpTreeView;
   FileSysTreeModel        *mpTreeModel;
   FileSysTreeModel        *mpNextTreeModel;
   FileSysTreeUpdate       *mpTreeUpdate;
   SearchWidget            *mpSearch;
   TrackInfoWidget         *mpTrackInfo;
   QTextBrowser            *mpHelpText;
   QSplitter               *mpSplitter;
   QSignalMapper           *mpFKeyMapper;
   QString                 mCurrentFile;
   QString                 mNextFile;
};

/*! @} */

#endif
