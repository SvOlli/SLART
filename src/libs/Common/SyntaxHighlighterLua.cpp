/*
 * src/libs/Common/CodeHighlighterLua.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

/* class declaration */
#include "SyntaxHighlighterLua.hpp"

/* system headers */

/* Qt headers */
#include <QStringList>
#include <QTextCharFormat>

/* local library headers */

/* local headers */
#include "SyntaxHighlightingRule.hpp"
#include "SyntaxHighlightingRuleComment.hpp"

/* class variables */



SyntaxHighlighterLua::SyntaxHighlighterLua( QTextDocument *parent )
: SyntaxHighlighter( parent )
{
   QTextCharFormat keywordFormat;
   keywordFormat.setForeground( Qt::darkBlue );

   QStringList keywordPatterns;
   keywordPatterns
         << "\\band\\b"       << "\\bbreak\\b"  << "\\bdo\\b"     << "\\belse\\b"
         << "\\belseif\\b"    << "\\bend\\b"    << "\\bfalse\\b"  << "\\bfor\\b"
         << "\\bfunction\\b"  << "\\bif\\b"     << "\\bin\\b"     << "\\blocal\\b"
         << "\\bnil\\b"       << "\\bnot\\b"    << "\\bor\\b"     << "\\brepeat\\b"
         << "\\breturn\\b"    << "\\bthen\\b"   << "\\btrue\\b"   << "\\buntil\\b"
         << "\\bwhile\\b";

   foreach( const QString &pattern, keywordPatterns )
   {
      addRule( pattern, keywordFormat );
   }

   QTextCharFormat classFormat;
   classFormat.setForeground( Qt::darkMagenta );
   addRule( "\\b[A-Za-z0-9_]+\\.\\b", classFormat );

   QTextCharFormat quotationFormat;
   quotationFormat.setForeground( Qt::darkGreen );
   addRule( "\".*\"", quotationFormat );

#if 0
   QTextCharFormat functionFormat;
   functionFormat.setForeground( Qt::blue );
   addRule( "\\b[A-Za-z0-9_]+ *(?=\\()", functionFormat );
#endif

   QTextCharFormat commentFormat;
   commentFormat.setForeground( Qt::gray );
   addRule( new SyntaxHighlightingRuleComment( QRegExp("--[^\n]*"), QRegExp("--\\[\\["), QRegExp("\\]\\]"), commentFormat, this ) );
}


SyntaxHighlighterLua::~SyntaxHighlighterLua()
{
}
