/**
 * src/libs/Common/LineEdit.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

/* class declaration */
#include "LineEdit.hpp"

/* system headers */

/* Qt headers */
#include <QtGui>

/* local library headers */

/* local headers */


LineEdit::LineEdit( QWidget *parent )
: QLineEdit( parent )
{
   setCompleter( new QCompleter( QStringList(), this ) );
   completer()->setCaseSensitivity( Qt::CaseInsensitive );
}


LineEdit::~LineEdit()
{
}


void LineEdit::setCompleterTexts( const QStringList &list )
{
   QStringListModel *model = qobject_cast<QStringListModel*>( completer()->model() );
   model->setStringList( list );
}


void LineEdit::addCompleterText( const QString &string )
{
   QStringListModel *model = qobject_cast<QStringListModel*>( completer()->model() );
   QStringList list( model->stringList() );
   if( string.isEmpty() )
   {
      if( list.contains( text() ) )
      {
         return;
      }
      list << text();
   }
   else
   {
      if( list.contains( string ) )
      {
         return;
      }
      list << string;
   }
   model->setStringList( list );
}
