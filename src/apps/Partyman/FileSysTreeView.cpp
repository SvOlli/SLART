/**
 * src/apps/Partyman/FileSysTreeView.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "FileSysTreeView.hpp"

/* system headers */

/* Qt headers */
#include <QContextMenuEvent>

/* local library headers */

/* local headers */


FileSysTreeView::FileSysTreeView( QWidget *parent )
: QTreeView( parent )
{
   setSelectionMode( QAbstractItemView::NoSelection );
   setIndentation( 8 );
   setMouseTracking( true );
   setUniformRowHeights ( true );
   connect(this, SIGNAL(entered(QModelIndex)),
           this, SLOT(setToolTip(QModelIndex)));
   connect(this, SIGNAL(doubleClicked(QModelIndex)),
           this, SLOT(addIfFile(QModelIndex)));
}


FileSysTreeView::~FileSysTreeView()
{
}


void FileSysTreeView::addIfFile( const QModelIndex &index )
{
   if( !index.child(0,0).isValid() )
   {
      emit context( index );
   }
}


void FileSysTreeView::contextMenuEvent( QContextMenuEvent *event )
{
   emit context( indexAt( event->pos() ) );
}


void FileSysTreeView::keyPressEvent( QKeyEvent *event )
{
   if( !event->isAutoRepeat() )
   {
      switch( event->key() )
      {
         case Qt::Key_Return:
         case Qt::Key_Enter:
            emit context( currentIndex() );
            break;
         default:
            break;
      }
   }
   QTreeView::keyPressEvent( event );
}


void FileSysTreeView::setToolTip( const QModelIndex &index )
{
   if( visualRect( index ).width() < sizeHintForIndex( index ).width() )
   {
      QTreeView::setToolTip( index.data().toString() );
   }
   else
   {
      QTreeView::setToolTip( QString() );
   }
}
