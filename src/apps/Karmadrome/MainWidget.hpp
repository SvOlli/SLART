/**
 * src/apps/Karmadrome/MainWidget.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef MAINWIDGET_HPP
#define MAINWIDGET_HPP MAINWIDGET_HPP

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
class Database;
class GenericSatMsgHandler;
class ImportExport;
class Satellite;
class ScrollLine;


class MainWidget : public QWidget
{
Q_OBJECT
   
public:
   MainWidget( QWidget *parent = 0, Qt::WindowFlags flags = 0 );
   virtual ~MainWidget();
   
public slots:
   /* add currently played track to the list */
   void addToList( QWidget *widget );
   /* handle SLART message */
   void handleSatellite( const QByteArray &msg );
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
   /* sync TrackInfo data */
   void updateTrackInfo( const TrackInfo &trackInfo );
   
signals:
   void requestChangeTitle( const QIcon &icon, const QString &title );
   
private:
   MainWidget( const MainWidget &other );
   MainWidget &operator=( const MainWidget &other );
   
   Database             *mpDatabase;
   Satellite            *mpSatellite;
   GenericSatMsgHandler *mpGenericSatMsgHandler;
   ImportExport         *mpImportExport;
   ScrollLine           *mpFileName;
   TrackInfoWidget      *mpTrackInfo;
   QPushButton          *mpReadButton;
   QPushButton          *mpExportButton;
   QMenu                *mpExportMenu;
   QAction              *mpExportFavorite;
   QAction              *mpExportUnwanted;
   QPushButton          *mpImportButton;
   QMenu                *mpImportMenu;
   QAction              *mpImportFavorite;
   QAction              *mpImportUnwanted;
   ButtonsWidget        *mpListButtons;
   
   QPushButton          *mpSettingsButton;
   QPushButton          *mpAddButton;
   QPushButton          *mpRemoveButton;
   QMenu                *mpRemoveMenu;
   ConfigDialog         *mpConfigDialog;
   QTimer               *mpTimer;
   
   QStringList          mPlaylists;
   TrackInfo            mTrackInfo;
};

#endif
