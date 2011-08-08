/**
 * src/apps/Partyman/PlaylistContentWidget.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "PlaylistContentWidget.hpp"

/* system headers */

/* Qt headers */
#include <QtGui>

/* local library headers */
#include <Database.hpp>
#include <GlobalConfigWidget.hpp>
#include <MySettings.hpp>
#include <Trace.hpp>

/* local headers */
#include "TrackInfoListModel.hpp"


PlaylistContentWidget::PlaylistContentWidget( Database *database, bool allowResort, QWidget *parent )
: QListView( parent )
, mpDatabase( database )
, mpPlaylistModel( new TrackInfoListModel( mpDatabase, this ) )
{
   setEditTriggers( QAbstractItemView::NoEditTriggers );
   setMouseTracking( true );
   setAlternatingRowColors( true );
   setSelectionMode( QAbstractItemView::ExtendedSelection );
   setDragEnabled( true );
   setAcceptDrops( true );
   setDropIndicatorShown( true );
   viewport()->setAcceptDrops( true );
   setModel( mpPlaylistModel );
   if( allowResort )
   {
      setDragDropMode( QAbstractItemView::DragDrop );
   }
   else
   {
      setDragDropMode( QAbstractItemView::DragOnly );
   }
#if USE_DRAG_WORKAROUND
#else
   setDefaultDropAction( Qt::MoveAction );
#endif
   connect( this, SIGNAL(clicked(QModelIndex)),
            this, SLOT(handleClick(QModelIndex)) );
   connect( this, SIGNAL(doubleClicked(QModelIndex)),
            this, SLOT(handleDoubleClick(QModelIndex)) );
   connect( mpPlaylistModel, SIGNAL(rowsRemoved(QModelIndex,int,int)),
            this, SIGNAL(dataRemoved()) );
}


PlaylistContentWidget::~PlaylistContentWidget()
{
}


void PlaylistContentWidget::addItems( const QStringList &items, bool atStart )
{
   if( !items.size() )
   {
      return;
   }
   QModelIndex idx;
   int startRow = atStart ? 0 : mpPlaylistModel->rowCount();
   mpPlaylistModel->insertRows( startRow, items.count() );
   for( int i = 0; i < items.count(); i++ )
   {
      idx = mpPlaylistModel->index( startRow + i );
      mpPlaylistModel->setData( idx, items.at(i) );
   }
}


void PlaylistContentWidget::removeSelectedItems( QStringList *list )
{
   QModelIndexList indexes( selectedIndexes() );
   if (indexes.count() > 0)
   {
      QList<QPersistentModelIndex> persistentIndexes;

      for (int i = 0; i<indexes.count(); i++)
      {
         persistentIndexes.append( indexes.at(i) );
      }

      QPersistentModelIndex idx;
      for (int i = 0; i<persistentIndexes.count(); i++)
      {
         idx = persistentIndexes.at(i);
         if( idx.isValid() )
         {
            if( list )
            {
               (*list) << mpPlaylistModel->data( idx, Qt::ToolTipRole ).toString();
            }
            mpPlaylistModel->removeRow( idx.row(), idx.parent() );
         }
         else
         {
            mpPlaylistModel->removeRow( idx.row(), QModelIndex() );
         }
      }
   }
}


QStringList PlaylistContentWidget::allFilePaths()
{
   TrackInfoList til( mpPlaylistModel->list() );
   QStringList filePaths;
   foreach( const TrackInfo &trackInfo, til )
   {
      filePaths << trackInfo.mDirectory + "/" + trackInfo.mFileName;
   }
   return filePaths;
}


QString PlaylistContentWidget::takeFilePath( int i )
{
   QModelIndex idx( mpPlaylistModel->index( i ) );
   QString filePath( mpPlaylistModel->data( idx, Qt::ToolTipRole ).toString() );
   mpPlaylistModel->removeRow( i );
   return filePath;
}


int PlaylistContentWidget::count()
{
   return mpPlaylistModel->rowCount();
}


void PlaylistContentWidget::clear()
{
   mpPlaylistModel->setList( TrackInfoList() );
}


void PlaylistContentWidget::keyPressEvent( QKeyEvent *event )
{
   if( !event->isAutoRepeat() )
   {
      emit context( currentIndex(), event->key() );
   }
   QListView::keyPressEvent( event );
}


void PlaylistContentWidget::handleClick( const QModelIndex &idx )
{
   GlobalConfigWidget::setClipboard( mpPlaylistModel->data( idx, Qt::ToolTipRole ).toString() );
}


void PlaylistContentWidget::handleDoubleClick( const QModelIndex &idx )
{
   emit context( idx, 0 );
}


void PlaylistContentWidget::contextMenuEvent( QContextMenuEvent *event )
{
   emit context( indexAt( event->pos() ), 0 );
}


#if USE_DRAG_WORKAROUND
void PlaylistContentWidget::startDrag( Qt::DropActions supportedActions )
{
   QModelIndexList indexes( selectedIndexes() );
   if (indexes.count() > 0)
   {
      QList<QPersistentModelIndex> persistentIndexes;
      QMimeData *data = mpPlaylistModel->mimeData( indexes );
      if (!data)
      {
         return;
      }
      for (int i = 0; i<indexes.count(); i++)
      {
         persistentIndexes.append( QPersistentModelIndex( indexes.at(i) ) );
      }

      QDrag *drag = new QDrag( this );
#if 0
      QImage image;
      QPixmap pixmap( QPixmap::fromImage( image ) );
      drag->setPixmap( pixmap );
#endif
      drag->setMimeData( data );
      drag->setHotSpot( QPoint(0, 0) );

      Qt::DropAction defaultDropAction = Qt::IgnoreAction;
      if( (supportedActions & Qt::MoveAction) &&
          (dragDropMode() != QAbstractItemView::InternalMove) )
      {
         defaultDropAction = Qt::MoveAction;
      }

      if ( drag->exec( supportedActions, defaultDropAction ) == Qt::MoveAction )
      {
         QPersistentModelIndex idx;
         for (int i = 0; i<indexes.count(); i++)
         {
            idx = persistentIndexes.at(i);
            if (idx.isValid())
            {
               mpPlaylistModel->removeRow( idx.row(), idx.parent() );
            }
            else
            {
               mpPlaylistModel->removeRow( idx.row(), QModelIndex() );
            }
         }
      }
   }
}
#endif
