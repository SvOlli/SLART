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
#include "GlobalConfigWidget.hpp"
#include "MySettings.hpp"

#include "Trace.hpp"


PlaylistContentWidget::PlaylistContentWidget( Database *database, bool allowResort, QWidget *parent )
: QListWidget( parent )
, mpDatabase( database )
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
   connect( this, SIGNAL(itemClicked(QListWidgetItem *)),
            this, SLOT(handleClick(QListWidgetItem *)) );
   connect( this, SIGNAL(itemDoubleClicked(QListWidgetItem *)),
            this, SLOT(handleDoubleClick(QListWidgetItem *)) );
}


void PlaylistContentWidget::addItems( const QStringList &items, bool atStart )
{
   int i;
   TrackInfoList trackInfoList;
   for( i = 0; i < items.count(); i++ )
   {
      TrackInfo trackInfo;
      if( !mpDatabase->getTrackInfo( &trackInfo, items.at(i) ) )
      {
         int lastSlash = items.at(i).lastIndexOf("/");
         trackInfo.mDirectory = items.at(i).left(lastSlash);
         trackInfo.mFileName  = items.at(i).mid(lastSlash+1);
      }
      trackInfoList.append( trackInfo );
   }
   addItems( trackInfoList, atStart );
}


void PlaylistContentWidget::addItems( const TrackInfoList &trackInfoList, bool atStart )
{
   QString pattern( MySettings().value( "ListPattern","(|$PLAYTIME|)|$ARTIST| - |$TITLE|" ).toString() );
   int i;
   for( i = 0; i < trackInfoList.count(); i++ )
   {
      QListWidgetItem *item = 0;
      if( !trackInfoList.at(i).mArtist.isEmpty() && !trackInfoList.at(i).mTitle.isEmpty() )
      {
         item = new QListWidgetItem( trackInfoList.at(i).displayString( pattern ) );
      }
      else
      {
         item = new QListWidgetItem( trackInfoList.at(i).mFileName );
      }
      item->setToolTip( trackInfoList.at(i).filePath() );
      insertItem( atStart ? 0 : count(), item );
   }
}


void PlaylistContentWidget::handleClick( QListWidgetItem *item )
{
   GlobalConfigWidget::setClipboard( item->toolTip() );
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
         (*list) << item->toolTip();
      }
      delete item;
   }
   if( row >= 0 )
   {
      scrollToItem( item(row) );
      // scroll to bottom if too big (todo)
   }
}


void PlaylistContentWidget::mouseReleaseEvent( QMouseEvent *event )
{
   QListWidget::mouseReleaseEvent( event );
   mLeftButton = (event->buttons() & Qt::LeftButton);
}


void PlaylistContentWidget::mouseMoveEvent( QMouseEvent *event )
{
   QListWidget::mouseMoveEvent( event );
   mLeftButton = (event->buttons() & Qt::LeftButton);
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
