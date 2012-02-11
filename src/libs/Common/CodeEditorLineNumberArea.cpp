/*
 * src/libs/Common/CodeEditorLineNumberArea.cpp
 * written by Sven Oliver Moll
 *
 * based upon the code editor example from the Qt documentation
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

/* class declaration */
#include "CodeEditorLineNumberArea.hpp"

/* system headers */

/* Qt headers */

/* local library headers */

/* local headers */
#include "CodeEditor.hpp"


CodeEditorLineNumberArea::CodeEditorLineNumberArea( CodeEditor *codeEditor )
: QWidget( codeEditor )
, mpCodeEditor( codeEditor )
{
}


CodeEditorLineNumberArea::~CodeEditorLineNumberArea()
{
}


QSize CodeEditorLineNumberArea::sizeHint() const
{
   return QSize( mpCodeEditor->lineNumberAreaWidth(), 0 );
}


void CodeEditorLineNumberArea::paintEvent( QPaintEvent *event )
{
   mpCodeEditor->lineNumberAreaPaintEvent(event);
}
