/**
 * PlaylistContentWidget.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#include <QContextMenuEvent>
#include <QScrollBar>
#include <QApplication>
#include <QClipboard>

#include "PlaylistContentWidget.hpp"

#include "Trace.hpp"


PlaylistContentWidget::PlaylistContentWidget( bool allowResort, QWidget *parent )
: QListWidget( parent )
, mMovingItem( false )
, mLeftButton( false )
{
   setSelectionMode( QAbstractItemView::ExtendedSelection );
   setSortingEnabled ( false );
   setMouseTracking( true );
   if( allowResort )
   {
      connect( this, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),
               this, SLOT(moveItem(QListWidgetItem*,QListWidgetItem*)));
   }
   connect( this, SIGNAL(entered(QModelIndex)), this, SLOT(setToolTip(QModelIndex)) );
   connect( this, SIGNAL(itemClicked(QListWidgetItem *)),
            this, SLOT(handleClick(QListWidgetItem *)) );
   connect( this, SIGNAL(itemDoubleClicked(QListWidgetItem *)),
            this, SLOT(handleDoubleClick(QListWidgetItem *)) );
}


void PlaylistContentWidget::handleClick( QListWidgetItem *item )
{
   QClipboard *clipboard = QApplication::clipboard();
   clipboard->setText( item->text(), QClipboard::Selection );
#if 0
   clipboard->setText( item->text(), QClipboard::Clipboard );
#endif
}


void PlaylistContentWidget::handleDoubleClick( QListWidgetItem *item )
{
   emit context( indexFromItem( item ), 0 );
}


void PlaylistContentWidget::contextMenuEvent( QContextMenuEvent *event )
{
   emit context( indexAt(event->pos()), 0 );
}


void PlaylistContentWidget::removeSelectedItems( QStringList *list )
{
   QList<QListWidgetItem *> items = selectedItems();
   int row = -1;
   if( items.size() > 0 )
   {
      row = indexFromItem( items.at(0) ).row();
   }
   for(int i = 0; i < items.size(); i++ )
   {
      QListWidgetItem *item = takeItem( indexFromItem( items.at(i) ).row() );
      if( list )
      {
         (*list) << item->data(Qt::DisplayRole).toString();
      }
      delete item;
   }
   if( row >= 0 )
   {
      scrollToItem( item(row) );
      // scroll to bottom if too big (todo)
   }
   QListWidget::setToolTip( QString() );
}


void PlaylistContentWidget::mouseReleaseEvent(QMouseEvent *event)
{
   QListWidget::mouseReleaseEvent( event );
   mLeftButton = (event->buttons() & Qt::LeftButton);
}


void PlaylistContentWidget::mouseMoveEvent(QMouseEvent *event)
{
   QListWidget::mouseMoveEvent( event );
   mLeftButton = (event->buttons() & Qt::LeftButton);
   
   if( !indexAt(event->pos()).isValid() )
   {
      QListWidget::setToolTip( QString() );
   }
}


void PlaylistContentWidget::moveItem( QListWidgetItem *a, QListWidgetItem *b )
{
   int rowa = row(a);
   int rowb = row(b);
   
   if( !a || !b || mMovingItem || !mLeftButton )
   {
      return;
   }
   
   mMovingItem = true;
   QListWidgetItem *temp = takeItem( rowb );
   insertItem( rowa, temp );
   setCurrentItem( temp );
   mMovingItem = false;
}


void PlaylistContentWidget::keyPressEvent( QKeyEvent *event )
{
   if( !event->isAutoRepeat() )
   {
      emit context( currentIndex(), event->key() );
   }
   QListWidget::keyPressEvent( event );
}


void PlaylistContentWidget::setToolTip( const QModelIndex &index )
{
   if( horizontalScrollBar()->isVisible() )
   {
      QListWidget::setToolTip( index.data().toString() );
   }
   else
   {
      QListWidget::setToolTip( QString() );
   }
}
