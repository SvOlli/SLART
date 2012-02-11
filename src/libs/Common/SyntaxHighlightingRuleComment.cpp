/*
 * src/libs/Common/CodeHighlighterLua.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

/* class declaration */
#include "SyntaxHighlightingRuleComment.hpp"

/* system headers */

/* Qt headers */

/* local library headers */

/* local headers */
#include "SyntaxHighlighter.hpp"

/* class variables */


SyntaxHighlightingRuleComment::SyntaxHighlightingRuleComment( const QRegExp &singleLine,
                                                              const QRegExp &multiLineStart,
                                                              const QRegExp &multiLineEnd,
                                                              const QTextCharFormat &format,
                                                              SyntaxHighlighter *parent )
: SyntaxHighlightingRule( singleLine, format, parent )
, mMultiLineStart( multiLineStart )
, mMultiLineEnd( multiLineEnd )
{
}


SyntaxHighlightingRuleComment::~SyntaxHighlightingRuleComment()
{
}


void SyntaxHighlightingRuleComment::highlightBlock( const QString &text )
{
   mpParent->setCurrentBlockState(0);

   int startIndex = 0;
   bool previous = true;
   if (mpParent->previousBlockState() != 1)
   {
      startIndex = mExpression.indexIn( text );
      previous = false;
   }

   while (startIndex >= 0)
   {
      int endIndex = mMultiLineEnd.indexIn( text, startIndex );
      if( (mMultiLineStart.indexIn( text, startIndex ) != startIndex ) && !previous )
      {
         mpParent->setFormat( startIndex, mExpression.matchedLength(), mFormat );
         break;
      }
      int commentLength;
      if( endIndex == -1 )
      {
         mpParent->setCurrentBlockState(1);
         commentLength = text.length() - startIndex;
      }
      else
      {
         commentLength = endIndex - startIndex + mMultiLineEnd.matchedLength();
      }
      mpParent->setFormat( startIndex, commentLength, mFormat );
      startIndex = mExpression.indexIn( text, startIndex + commentLength );
   }
}
