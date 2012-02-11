/*
 * src/libs/Common/CodeEditorLineNumberArea.hpp
 * written by Sven Oliver Moll
 *
 * based upon the code editor example from the Qt documentation
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef CODEEDITORLINENUMBERAREA_HPP
#define CODEEDITORLINENUMBERAREA_HPP CODEEDITORLINENUMBERAREA_HPP

/* base class */
#include <QWidget>

/* system headers */

/* Qt headers */

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */
class QPaintEvent;
class QSize;

/* forward declaration of local classes */
class CodeEditor;


/*!
  \addtogroup Common
  @{
  */

/*!
 \brief helper class for CodeEditor to display line numbers

*/
class CodeEditorLineNumberArea : public QWidget
{
public:
   /*!
    \brief contructor

    \param codeEditor parent widget: code editor
   */
   CodeEditorLineNumberArea( CodeEditor *codeEditor );

   /*!
    \brief destructor

    */
   virtual ~CodeEditorLineNumberArea();

   /*!
    \brief get recommended size for the widget

    \return QSize
    */
   QSize sizeHint() const;

protected:
   /*!
    \brief reimplemented event handler

    \param event
    */
   void paintEvent( QPaintEvent *event );

private:
   Q_DISABLE_COPY( CodeEditorLineNumberArea )

   CodeEditor *mpCodeEditor; /*! \brief the parent code editor */
};

/*! @} */

#endif
