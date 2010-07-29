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
   /*  */
   QStringList allFilePaths();
   /*  */
   QString takeFilePath( int i );
   /*  */
   int count();
   /*  */
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
   /*  */
   void dataRemoved();

protected:
   /* catch right mouse button */
   virtual void contextMenuEvent( QContextMenuEvent *event );
   /* for handling key presses */
   virtual void keyPressEvent( QKeyEvent *event );

#if USE_DRAG_WORKAROUND
   /* workaround for missing setDefaultDropAction() */
   void startDrag( Qt::DropActions supportedActions );
#endif

private:
   PlaylistContentWidget( const PlaylistContentWidget &other );
   PlaylistContentWidget &operator=( const PlaylistContentWidget &other );

   Database             *mpDatabase;
   TrackInfoListModel   *mpPlaylistModel;

};

#endif
