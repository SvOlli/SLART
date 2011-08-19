/**
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


class PlaylistControlWidget : public QWidget
{
Q_OBJECT

public:
   PlaylistControlWidget( Database *database, ConfigDialog *config,
                   QWidget *parent = 0, Qt::WindowFlags f = 0 );
   virtual ~PlaylistControlWidget();

   /* get name of next track either from playlist */
   void getNextTrack( QString *fileName );
   /* set checkboxes of trackinfo */
   void setTrackInfoFavoriteUnwanted( bool favorite, bool unwanted );
   /* save the current and next track for later saving of complete playlist */
   void setCurrentNext( const QString &current = QString(),
                        const QString &next = QString() );

public slots:
   /* add internal entries */
   void addEntries( const QModelIndex &qmi );
   /* delete entries */
   void deleteEntry( const QModelIndex &qmi, int key );
   /* add external entries */
   void addEntries( const QStringList &entries, bool atStart = false );
   /* save current tab for restart */
   void handleTabChange( int tabNr );
   /* re-read config parameters */
   void readConfig();
   /* pass the track info through to track info */
   void getTrack( const TrackInfo &trackInfo );
   /* start the background update of the browser */
   void startBrowserUpdate();
   /* finish up the browser background update */
   void finishBrowserUpdate();
   /* pass the update through to track info */
   void updateTrackInfo();
   /* save the playlist to settings (prepend current and next, if any) */
   void savePlaylist();

signals:
   /* unfold a node */
   void expand( const QModelIndex &qmi );
   /* tell if the current playlist is valid */
   void playlistIsValid( bool isValid );

private:
   PlaylistControlWidget( const PlaylistControlWidget &that );
   PlaylistControlWidget &operator=( const PlaylistControlWidget &that );

   /* add entries to a stringlist */
   void addEntries( QStringList *list, const QModelIndex &qmi );

   /* get a random track from database */
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

#endif
