/*
 * src/libs/Common/CodeHighlighterLua.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

/* class declaration */
#include "SyntaxHighlightingRule.hpp"

/* system headers */

/* Qt headers */

/* local library headers */

/* local headers */
#include "SyntaxHighlighter.hpp"

/* class variables */


SyntaxHighlightingRule::SyntaxHighlightingRule( const QRegExp &expression,
                                                const QTextCharFormat &format,
                                                SyntaxHighlighter *parent )
: mpParent( parent )
, mFormat( format )
, mExpression( expression )
{
}


SyntaxHighlightingRule::~SyntaxHighlightingRule()
{
}


void SyntaxHighlightingRule::highlightBlock( const QString &text )
{
   int index = mExpression.indexIn(text);
   while( index >= 0 )
   {
      int length = mExpression.matchedLength();
      mpParent->setFormat( index, length, mFormat );
      index = mExpression.indexIn( text, index + length );
   }
}
