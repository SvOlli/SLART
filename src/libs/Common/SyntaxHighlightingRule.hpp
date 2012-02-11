/*
 * src/libs/Common/CodeHighlighterLua.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef SYNTAXHIGHLIGHTINGRULE_HPP
#define SYNTAXHIGHLIGHTINGRULE_HPP SYNTAXHIGHLIGHTINGRULE_HPP

/* base class */

/* system headers */

/* Qt headers */
#include <QRegExp>
#include <QTextCharFormat>

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */

/* forward declaration of local classes */
class SyntaxHighlighter;


/*!
  \addtogroup Common

  @{
*/

/*!
 \brief generic highlighting rule

*/
class SyntaxHighlightingRule
{
public:
   /*!
    \brief constructor

    \param expression expression for highlighting
    \param format format to apply
    \param parent parent syntax highlighter using this rule
   */
   SyntaxHighlightingRule( const QRegExp &expression,
                           const QTextCharFormat &format,
                           SyntaxHighlighter *parent = 0 );
   /*!
    \brief destructor

   */
   virtual ~SyntaxHighlightingRule();

   /*!
    \brief highlight text block

    \param text block to hightlight
   */
   virtual void highlightBlock( const QString &text );

private:
   Q_DISABLE_COPY( SyntaxHighlightingRule )

protected:
   SyntaxHighlighter *mpParent;   /*!< \brief parent syntax highlighter */
   QTextCharFormat   mFormat;     /*!< \brief format to apply */
   QRegExp           mExpression; /*!< \brief expression for highlighting */
};

/*! @} */

#endif
