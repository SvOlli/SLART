/*
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

/*!
  \addtogroup Partyman

  @{
*/

/*!
 \brief TODO

*/
class PlaylistContentWidget : public QListView
{
   Q_OBJECT

public:
   PlaylistContentWidget( Database *database, bool allowResort, QWidget *parent = 0 );
   virtual ~PlaylistContentWidget();

   /*!
    \brief remove selected items from list widget and put them into the list parameter

   */
   void removeSelectedItems( QStringList *list = 0 );
   /*!
    \brief get a list with all elements

   */
   QStringList allFilePaths();
   /*!
    \brief get and remove an element from the list

   */
   QString takeFilePath( int i );
   /*!
    \brief get number of elements in the list

   */
   int count();
   /*!
    \brief clear the list

   */
   void clear();

public slots:
   /*!
    \brief add tracks to the list

   */
   void addItems( const QStringList &items, bool atStart = false );
   /*!
    \brief handle single click

   */
   void handleClick( const QModelIndex &idx );
   /*!
    \brief handle double click

   */
   void handleDoubleClick( const QModelIndex &idx );

signals:
   /*!
    \brief signal that right mousebutton has been pressed

   */
   void context( const QModelIndex &index, int key );
   /*!
    \brief some data has been removed, so others can update the counter

   */
   void dataRemoved();
   /*!
    \brief content has changed, should be saved

   */
   void contentChanged();

protected:
   /*!
    \brief catch right mouse button

   */
   virtual void contextMenuEvent( QContextMenuEvent *event );
   /*!
    \brief for handling key presses

   */
   virtual void keyPressEvent( QKeyEvent *event );
   /*!
    \brief for detecting a drop event to save playlist

   */
   virtual void dropEvent( QDropEvent *event );

#if USE_DRAG_WORKAROUND
   /*!
    \brief workaround for missing setDefaultDropAction()

   */
   void startDrag( Qt::DropActions supportedActions );
#endif

private:
   PlaylistContentWidget( const PlaylistContentWidget &that );
   PlaylistContentWidget &operator=( const PlaylistContentWidget &that );

   Database             *mpDatabase;
   TrackInfoListModel   *mpPlaylistModel;

   QTimer               *mpTimer;
};

/*! @} */

#endif
