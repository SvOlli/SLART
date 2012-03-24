/*
 * src/libs/Common/CodeEditor.hpp
 * written by Sven Oliver Moll
 *
 * based upon the code editor example from the Qt documentation
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef CODEEDITOR_HPP
#define CODEEDITOR_HPP CODEEDITOR_HPP

/* base class */
#include <QPlainTextEdit>

/* system headers */

/* Qt headers */

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */
class QPaintEvent;
class QResizeEvent;
class QSize;
class QWidget;

/* forward declaration of local classes */
class CodeEditorLineNumberArea;
class SyntaxHighlighter;


/*!
  \addtogroup Common
  @{
  */

/*!
 \brief

*/
class CodeEditor : public QPlainTextEdit
{
   Q_OBJECT

public:
   /*!
    \brief

    \param parent
   */
   CodeEditor( QWidget *parent = 0 );

   /*!
    \brief

    \param parent
   */
   virtual ~CodeEditor();

   /*!
     \brief

     \param event
    */
   void lineNumberAreaPaintEvent( QPaintEvent *event );

   /*!
     \brief

     \return int
    */
   int lineNumberAreaWidth();

protected:
   /*!
     \brief

     \param event
    */
   void resizeEvent( QResizeEvent *event );

private slots:
   /*!
     \brief

     \param newBlockCount
    */
   void updateLineNumberAreaWidth( int newBlockCount );

   /*!
     \brief

    */
   void highlightCurrentLine();

   /*!
     \brief

     \param rect
     \param dy
    */
   void updateLineNumberArea( const QRect &rect, int dy );

private:
   CodeEditorLineNumberArea  *mpLineNumberArea; /*!< \todo */
};

/*! @} */

#endif
