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


LineEdit::LineEdit( QWidget *parent, bool autoScroll )
: QLineEdit( parent )
{
   setCompleter( new QCompleter( QStringList(), this ) );
   completer()->setCaseSensitivity( Qt::CaseInsensitive );
   completer()->setModelSorting( QCompleter::CaseInsensitivelySortedModel );
}


LineEdit::~LineEdit()
{
}


void LineEdit::setCompleterTexts( const QStringList &list )
{
   QStringListModel *model = qobject_cast<QStringListModel*>( completer()->model() );
   model->setStringList( list );
}


void LineEdit::addCompleterText( const QString &strings )
{
   QStringListModel *model = qobject_cast<QStringListModel*>( completer()->model() );
   QStringList list( model->stringList() );
   if( strings.isEmpty() )
   {
      list << text();
   }
   else
   {
      list << strings;
   }
   model->setStringList( list );
}
