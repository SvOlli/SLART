/*
 * src/libs/Common/CodeHighlighter.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef SYNTAXHIGHLIGHTER_HPP
#define SYNTAXHIGHLIGHTER_HPP SYNTAXHIGHLIGHTER_HPP

/* base class */
#include <QSyntaxHighlighter>

/* system headers */

/* Qt headers */

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */

/* forward declaration of local classes */
class SyntaxHighlightingRule;

/*!
  \addtogroup Common

  @{
*/

/*!
 \brief generic syntax highlighter for CodeEditor

*/
class SyntaxHighlighter : public QSyntaxHighlighter
{
   Q_OBJECT

public:
   /*!
    \brief constructor

    \param parent parent object
   */
   SyntaxHighlighter( QTextDocument *parent = 0 );
   /*!
    \brief destructor

   */
   virtual ~SyntaxHighlighter();

   /*!
    \brief add a syntax highlighting rule, which will be deleted on destruction of
           this object

    \param rule rule to add
   */
   void addRule( SyntaxHighlightingRule *rule );
   /*!
    \brief for convienience: create and add a rule

    \param pattern highlighing pattern
    \param format format to highlight to
   */
   void addRule( const QString &pattern, const QTextCharFormat &format );
   /*!
    \brief for convienience: create and add a rule

    \param pattern highlighing pattern
    \param format format to highlight to
   */
   void addRule( const QRegExp &pattern, const QTextCharFormat &format );

   /*!
    \brief callback for SyntaxHighlighingRule to access
           protected QSyntaxHighlighter::currentBlockState()

    \return see QSyntaxHighlighter::currentBlockState
   */
   int currentBlockState() const;
   /*!
    \brief callback for SyntaxHighlighingRule to access
           protected QSyntaxHighlighter::previousBlockState()

    \return see QSyntaxHighlighter::previousBlockState
   */
   int previousBlockState() const;
   /*!
    \brief callback for SyntaxHighlighingRule to access
           protected QSyntaxHighlighter::setCurrentBlockState()

    \param newState see QSyntaxHighlighter::setCurrentBlockState
   */
   void setCurrentBlockState( int newState );
   /*!
    \brief callback for SyntaxHighlighingRule to access
           protected QSyntaxHighlighter::setFormat()

    \param start see QSyntaxHighlighter::setCurrentBlockState
    \param count see QSyntaxHighlighter::setCurrentBlockState
    \param format see QSyntaxHighlighter::setCurrentBlockState
   */
   void setFormat( int start, int count, const QTextCharFormat &format );

protected:
   /*!
    \brief pass the text through the list of highlighting rules

    \param text
   */
   void highlightBlock( const QString &text );

private:
   Q_DISABLE_COPY( SyntaxHighlighter )

   QList<SyntaxHighlightingRule*> mRules; /*! \brief list of rules to walk through */
};

/*! @} */

#endif
