/*
 * src/apps/Partyman/SearchLineEdit.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */


/* class declaration */
#include "SearchLineEdit.hpp"

/* system headers */

/* Qt headers */
#include <QtGui>

/* local library headers */

/* local headers */


SearchLineEdit::SearchLineEdit( SearchWidget *parent )
: QLineEdit( parent )
, mpSearchWidget( parent )
{
}


void SearchLineEdit::keyPressEvent( QKeyEvent *event )
{
   if( event->key() == Qt::Key_Escape )
   {
      if( text().count() > 0 )
      {
         clear();
      }
      else
      {
         mpSearchWidget->selectedEntries( QModelIndex(), Qt::Key_Escape );
      }
   }
   QLineEdit::keyPressEvent( event );
}
