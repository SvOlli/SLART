/*
 * src/libs/Common/CodeHighlighterLua.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef SYNTAXHIGHLIGHTERLUA_HPP
#define SYNTAXHIGHLIGHTERLUA_HPP SYNTAXHIGHLIGHTERLUA_HPP

/* base class */
#include "SyntaxHighlighter.hpp"

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
 \brief contains Lua configuration for SyntaxHighlighter

*/
class SyntaxHighlighterLua : public SyntaxHighlighter
{
   Q_OBJECT

public:
   /*!
    \brief constructor

    \param parent parent object
   */
   SyntaxHighlighterLua( QTextDocument *parent = 0 );
   /*!
    \brief destructor

   */
   virtual ~SyntaxHighlighterLua();

private:
   Q_DISABLE_COPY( SyntaxHighlighterLua )
};

/*! @} */

#endif
