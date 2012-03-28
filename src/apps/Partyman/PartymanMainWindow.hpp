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
    \brief prepare startup of application

   */
   void startUp();

   void getNextTrack( QString *fileName );
   void setTrackInfoFavoriteUnwanted( bool favorite, bool unwanted );
   void setCurrentNext( const QString &current = QString(),
                        const QString &next = QString() );

protected:

public slots:
   /*!
    \brief handle request for new icon and title

    \param icon
    \param title
   */
   void changeTitle( const QIcon &icon, const QString &title );
   /*!
    \brief don't autostart if database is empty

    \param allow
   */
   void allowAutostart( bool allow );
   /*!
    \brief prohibit closing of window (Partyman kiosk mode)

   */
   void prohibitClose( bool prohibit );

   void addEntries( const QModelIndex &qmi );
   void deleteEntry( const QModelIndex &qmi, int key );
   void addEntries( const QStringList &entries, bool atStart = false );
   void readConfig();
   void getTrack( const TrackInfo &trackInfo );
   void startBrowserUpdate();
   void finishBrowserUpdate();
   void updateTrackInfo();
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

   bool                    mAllowAutostart; /*!< \todo */
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
