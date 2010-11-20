/**
 * src/apps/Rubberbandman/MyTreeView.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "MyTreeView.hpp"

/* system headers */

/* Qt headers */
#include <QKeyEvent>

/* local library headers */

/* local headers */


MyTreeView::MyTreeView( QWidget *parent )
: QTreeView( parent )
{
}


MyTreeView::~MyTreeView()
{
}


void MyTreeView::keyPressEvent( QKeyEvent *event )
{
   if( !event->isAutoRepeat() )
   {
      switch( event->key() )
      {
         case Qt::Key_Return:
         case Qt::Key_Enter:
            emit clicked( currentIndex() );
            break;
         default:
            break;
      }
   }
   QTreeView::keyPressEvent( event );
}
