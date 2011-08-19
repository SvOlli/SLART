/**
 * src/apps/Partyman/PlaylistContentWidget.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef PLAYLISTCONTENTWIDGET_HPP
#define PLAYLISTCONTENTWIDGET_HPP PLAYLISTCONTENTWIDGET_HPP

/* base class */
#include <QListView>

/* system headers */

/* Qt headers */

/* local library headers */
#include <TrackInfo.hpp>

/* local headers */

/* forward declaration of Qt classes */
class QDragEnterEvent;
class QDropEvent;
class QTimer;

/* forward declaration of local classes */
class Database;
class PlaylistModel;
class TrackInfoListModel;

#if QT_VERSION >= 0x040600
# define USE_DRAG_WORKAROUND 0
#else
# define USE_DRAG_WORKAROUND 1
#endif

class PlaylistContentWidget : public QListView
{
   Q_OBJECT

public:
   PlaylistContentWidget( Database *database, bool allowResort, QWidget *parent = 0 );
   virtual ~PlaylistContentWidget();

   /* remove selected items from list widget and put them into the list parameter */
   void removeSelectedItems( QStringList *list = 0 );
   /* get a list with all elements */
   QStringList allFilePaths();
   /* get and remove an element from the list */
   QString takeFilePath( int i );
   /* get number of elements in the list */
   int count();
   /* clear the list */
   void clear();

public slots:
   /* add tracks to the list */
   void addItems( const QStringList &items, bool atStart = false );
   /* handle single click */
   void handleClick( const QModelIndex &idx );
   /* handle double click */
   void handleDoubleClick( const QModelIndex &idx );

signals:
   /* signal that right mousebutton has been pressed */
   void context( const QModelIndex &index, int key );
   /* some data has been removed, so others can update the counter */
   void dataRemoved();
   /* content has changed, should be saved */
   void contentChanged();

protected:
   /* catch right mouse button */
   virtual void contextMenuEvent( QContextMenuEvent *event );
   /* for handling key presses */
   virtual void keyPressEvent( QKeyEvent *event );
   /* for detecting a drop event to save playlist */
   virtual void dropEvent( QDropEvent *event );

#if USE_DRAG_WORKAROUND
   /* workaround for missing setDefaultDropAction() */
   void startDrag( Qt::DropActions supportedActions );
#endif

private:
   PlaylistContentWidget( const PlaylistContentWidget &that );
   PlaylistContentWidget &operator=( const PlaylistContentWidget &that );

   Database             *mpDatabase;
   TrackInfoListModel   *mpPlaylistModel;

   QTimer               *mpTimer;
};

#endif
