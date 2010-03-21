/**
 * src/apps/Stripped/CDEditCheckBox.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "CDEditCheckBox.hpp"

/* system headers */

/* Qt headers */
#include <QtGui>

/* local library headers */

/* local headers */
#include "CDEdit.hpp"


CDEditCheckBox::CDEditCheckBox( CDEdit *parent )
: QCheckBox( parent )
, mpParent( parent )
{
}


CDEditCheckBox::CDEditCheckBox( const QString &text, CDEdit *parent )
: QCheckBox( text, parent )
, mpParent( parent )
{
}


void CDEditCheckBox::keyPressEvent( QKeyEvent *event )
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
   QCheckBox::keyPressEvent( event );
}
