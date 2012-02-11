/*
 * src/apps/Stripped/CDEditLineEdit.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "CDEditLineEdit.hpp"

/* system headers */

/* Qt headers */
#include <QtGui>

/* local library headers */

/* local headers */
#include "CDEdit.hpp"


CDEditLineEdit::CDEditLineEdit( CDEdit *parent )
: QLineEdit( parent )
, mpParent( parent )
{
}


CDEditLineEdit::CDEditLineEdit( const QString &text, CDEdit *parent )
: QLineEdit( text, parent )
, mpParent( parent )
{
}


void CDEditLineEdit::keyPressEvent( QKeyEvent *event )
{
   if( event->key() == Qt::Key_Up )
   {
      mpParent->keyUpDown( this, true );
      return;
   }
   if( event->key() == Qt::Key_Down )
   {
      mpParent->keyUpDown( this, false );
      return;
   }
   QLineEdit::keyPressEvent( event );
}
