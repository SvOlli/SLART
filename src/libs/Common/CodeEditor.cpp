/*
 * src/libs/Common/CodeEditor.cpp
 * written by Sven Oliver Moll
 *
 * based upon the code editor example from the Qt documentation
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

/* class declaration */
#include "CodeEditor.hpp"

/* system headers */

/* Qt headers */
#include <QPainter>

/* local library headers */

/* local headers */
#include "CodeEditorLineNumberArea.hpp"
#include "SyntaxHighlighterLua.hpp"


CodeEditor::CodeEditor( QWidget *parent )
: QPlainTextEdit( parent )
, mpLineNumberArea(  new CodeEditorLineNumberArea( this ) )
{
   QFont font( "Courier" );
   font.setBold( true );
   setFont( font );
#if 0
   QTextCharFormat format;
   format.setForeground( Qt::black );
   format.setFontWeight( QFont::Bold );
   setCurrentCharFormat( format );
#endif

   connect( this, SIGNAL(blockCountChanged(int)),
            this, SLOT(updateLineNumberAreaWidth(int)) );
   connect( this, SIGNAL(updateRequest(const QRect &, int)),
            this, SLOT(updateLineNumberArea(const QRect &, int)) );
   connect( this, SIGNAL(cursorPositionChanged()),
            this, SLOT(highlightCurrentLine()) );

   updateLineNumberAreaWidth(0);
   highlightCurrentLine();
}


CodeEditor::~CodeEditor()
{
}


int CodeEditor::lineNumberAreaWidth()
{
   int digits = 1;
   for( int max = qMax(1, blockCount()); max >= 10; max/= 10 )
   {
      ++digits;
   }

   return 3 + fontMetrics().width(QLatin1Char('9')) * digits;
}


void CodeEditor::updateLineNumberAreaWidth( int /* newBlockCount */ )
{ 
   setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}


void CodeEditor::updateLineNumberArea( const QRect &rect, int dy )
{
   if( dy )
   {
      mpLineNumberArea->scroll(0, dy);
   }
   else
   {
      mpLineNumberArea->update(0, rect.y(), mpLineNumberArea->width(), rect.height());
   }

   if (rect.contains(viewport()->rect()))
   {
      updateLineNumberAreaWidth(0);
   }
}


void CodeEditor::resizeEvent( QResizeEvent *event )
{
   QPlainTextEdit::resizeEvent( event );

   QRect cr( contentsRect() );
   cr.setWidth( lineNumberAreaWidth() );
   mpLineNumberArea->setGeometry( cr );
}


void CodeEditor::highlightCurrentLine()
{
   QList<QTextEdit::ExtraSelection> extraSelections;

   if( !isReadOnly() )
   {
      QTextEdit::ExtraSelection selection;

      QColor lineColor( QColor( Qt::yellow ).lighter( 150 ) );

      selection.format.setBackground( lineColor );
      selection.format.setProperty( QTextFormat::FullWidthSelection, true );
      selection.cursor = textCursor();
      selection.cursor.clearSelection();
      extraSelections.append( selection );
   }

   setExtraSelections( extraSelections );
}


void CodeEditor::lineNumberAreaPaintEvent( QPaintEvent *event )
{
   QPainter painter( mpLineNumberArea );
   painter.fillRect( event->rect(), Qt::lightGray );

   QTextBlock block( firstVisibleBlock() );
   int blockNumber( block.blockNumber() );
   int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
   int bottom = top + (int) blockBoundingRect(block).height();

   while( block.isValid() && top <= event->rect().bottom() )
   {
      if( block.isVisible() && bottom >= event->rect().top() )
      {
         QString number = QString::number(blockNumber + 1);
         painter.setPen(Qt::black);
         painter.drawText(0, top, mpLineNumberArea->width(), fontMetrics().height(),
                          Qt::AlignRight, number);
      }

      block = block.next();
      top = bottom;
      bottom = top + (int) blockBoundingRect(block).height();
      ++blockNumber;
   }
}
