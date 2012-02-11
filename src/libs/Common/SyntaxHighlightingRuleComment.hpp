/*
 * src/libs/Common/CodeHighlighterLua.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef SYNTAXHIGHLIGHTINGRULECOMMENT_HPP
#define SYNTAXHIGHLIGHTINGRULECOMMENT_HPP SYNTAXHIGHLIGHTINGRULECOMMENT_HPP

/* base class */
#include "SyntaxHighlightingRule.hpp"

/* system headers */

/* Qt headers */

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */

/* forward declaration of local classes */


/*!
  \addtogroup Common

  @{
*/

/*!
 \brief special highlighting rule that also covers multiline comments

*/
class SyntaxHighlightingRuleComment : public SyntaxHighlightingRule
{
public:
   /*!
    \brief constructor

    \param singleLine expression for single line comment
    \param multiLineStart start of multi line comment
    \param multiLineEnd end of multi line comment
    \param format format to apply
    \param parent parent syntax highlighter using this rule
   */
   SyntaxHighlightingRuleComment( const QRegExp &singleLine,
                                  const QRegExp &multiLineStart,
                                  const QRegExp &multiLineEnd,
                                  const QTextCharFormat &format,
                                  SyntaxHighlighter *parent = 0 );
   /*!
    \brief destructor

   */
   virtual ~SyntaxHighlightingRuleComment();

   /*!
    \brief highlight text block

    \param text block to hightlight
   */
   void highlightBlock( const QString &text );

private:
   Q_DISABLE_COPY( SyntaxHighlightingRuleComment )

protected:
   QRegExp           mMultiLineStart; /*!< \brief expression for start of multiline comment */
   QRegExp           mMultiLineEnd;   /*!< \brief expression for end of multiline comment */
};

/*! @} */

#endif
