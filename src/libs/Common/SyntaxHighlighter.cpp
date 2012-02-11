/*
 * src/libs/Common/CodeHighlighterLua.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

/* class declaration */
#include "SyntaxHighlighter.hpp"

/* system headers */

/* Qt headers */

/* local library headers */

/* local headers */
#include "SyntaxHighlightingRule.hpp"

/* class variables */



SyntaxHighlighter::SyntaxHighlighter( QTextDocument *parent )
: QSyntaxHighlighter( parent )
, mRules()
{
}


SyntaxHighlighter::~SyntaxHighlighter()
{
   foreach( SyntaxHighlightingRule* rule, mRules )
   {
      delete rule;
   }
}


void SyntaxHighlighter::addRule( SyntaxHighlightingRule *rule )
{
   mRules.append( rule );
}


void SyntaxHighlighter::addRule( const QString &pattern, const QTextCharFormat &format )
{
   addRule( new SyntaxHighlightingRule( QRegExp(pattern), format, this ) );
}


void SyntaxHighlighter::addRule( const QRegExp &pattern, const QTextCharFormat &format )
{
   addRule( new SyntaxHighlightingRule( pattern, format, this ) );
}


int SyntaxHighlighter::currentBlockState() const
{
   return QSyntaxHighlighter::currentBlockState();
}


int SyntaxHighlighter::previousBlockState() const
{
   return QSyntaxHighlighter::previousBlockState();
}


void SyntaxHighlighter::setCurrentBlockState( int newState )
{
   QSyntaxHighlighter::setCurrentBlockState( newState );
}


void SyntaxHighlighter::setFormat( int start, int count, const QTextCharFormat &format )
{
   QSyntaxHighlighter::setFormat( start, count, format );
}


void SyntaxHighlighter::highlightBlock( const QString &text )
{
   foreach( SyntaxHighlightingRule *rule, mRules )
   {
      rule->highlightBlock( text );
   }
}
