/**
 * MainWidget.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 */

#ifndef MAINWIDGET_HPP
#define MAINWIDGET_HPP MAINWIDGET_HPP

#include <QWidget>

#include "SLARTCom.hpp"
#include "TrackInfoWidget.hpp"

class QIcon;
class QString;
class QPushButton;
class QMenu;
class QLineEdit;
class QAction;
class QTimer;
class ButtonsWidget;
class ConfigDialog;
class ScrollLine;
class Database;


class MainWidget : public QWidget
{
Q_OBJECT

public:
   MainWidget( QWidget *parent = 0, Qt::WindowFlags flags = 0 );

public slots:
   /* add currently played track to the list */
   void addToList( QWidget *widget );
   /* handle SLART message */
   void handleSLART( const QStringList &message );
   /* handle adding of new playlist */
   void handleAdd();
   /* handle menu to export of playlist to m3u file */
   void handleExport( QAction *action );
   /* handle menu to import of playlist from m3u file */
   void handleImport( QAction *action );
   /* handle removal of playlist */
   void handleRemove( QAction *action );
   /* handle "Read Clipboard" button */
   void handleReadButton();
   /* label "Read Clipboard" button according to settings */
   void labelReadButton();
   /* update the lists containing the playlists */
   void updateLists();
   /* slot for timer to send out k0u notification */
   void sendK0u();
   /* export a playlist to m3u file */
   void exportM3u( const QString &folder, const QString &fileName );
   /* import a playlist from m3u file */
   void importM3u( const QString &folder, const QString &fileName );

signals:
   void requestChangeTitle( const QIcon &icon, const QString &title );

private:
   MainWidget( const MainWidget &other );
   MainWidget &operator=( const MainWidget &other );

   Database        *mpDatabase;
   ScrollLine      *mpFileName;
   TrackInfoWidget *mpTrackInfo;
   QPushButton     *mpReadButton;
   QPushButton     *mpExportButton;
   QMenu           *mpExportMenu;
   QAction         *mpExportFavorite;
   QAction         *mpExportUnwanted;
   QPushButton     *mpImportButton;
   QMenu           *mpImportMenu;
   ButtonsWidget   *mpListButtons;

   QPushButton     *mpSettingsButton;
   QPushButton     *mpAddButton;
   QPushButton     *mpRemoveButton;
   QMenu           *mpRemoveMenu;
   ConfigDialog    *mpConfigDialog;
   QTimer          *mpTimer;

   SLARTCom        mSLARTCom;
   QStringList     mPlaylists;
   TrackInfo       mTrackInfo;
};

#endif
