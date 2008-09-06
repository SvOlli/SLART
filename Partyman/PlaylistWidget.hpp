/**
 * PlaylistWidget.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#ifndef PLAYLISTWIDGET_HPP
#define PLAYLISTWIDGET_HPP PLAYLISTWIDGET_HPP

#include "FileSysTreeView.hpp"
#include "FileSysTreeUpdate.hpp"

#include <QWidget>
#include <QStringList>


class QLabel;
class QTabWidget;
class QSplitter;
class QTextBrowser;
   
class FileSysTreeModel;
class PlaylistContentWidget;
class SearchWidget;
class TrackInfo;
class TrackInfoWidget;
class ConfigDialog;
class Database;


class PlaylistWidget : public QWidget
{
Q_OBJECT

public:
   PlaylistWidget( Database *database, ConfigDialog *config,
                   QWidget *parent = 0, Qt::WindowFlags f = 0 );
   virtual ~PlaylistWidget();

   /* get name of next track either from playlist */
   void getNextTrack( QString *fileName );

protected:
   /* for drag & drop from external sources */
   void dragEnterEvent( QDragEnterEvent *event );
   /* for drag & drop from external sources */
   void dropEvent( QDropEvent *event );
   
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
   
signals:
   /* unfold a node */
   void expand( const QModelIndex &qmi );
   /* tell if the current playlist is valid */
   void playlistIsValid( bool isValid );

private:
   PlaylistWidget( const PlaylistWidget &other );
   PlaylistWidget &operator=( const PlaylistWidget &other );

   /* add entries to a stringlist */
   void addEntries( QStringList *list, const QModelIndex &qmi );
   
   /* get a random track from database */
   bool getRandomTrack( QString *fileName, QStringList *playedArtists, int randomTries,
                        bool favoriteOnly, bool leastPlayed, const QString &playFolder );

   Database              *mpDatabase;
   ConfigDialog          *mpConfig;
   QTabWidget            *mpTabs;
   PlaylistContentWidget *mpPlaylistContent;
   FileSysTreeView       *mpTreeView;
   FileSysTreeModel      *mpTreeModel;
   FileSysTreeModel      *mpNextTreeModel;
   FileSysTreeUpdate     *mpTreeUpdate;
   SearchWidget          *mpSearch;
   TrackInfoWidget       *mpTrackInfo;
   QTextBrowser          *mpHelpText;
   QSplitter             *mpSplitter;
};

#endif
