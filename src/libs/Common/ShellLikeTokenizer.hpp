/*
 * src/libs/Common/ShellLikeTokenizer.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef SHELLLIKETOKENIZER_HPP
#define SHELLLIKETOKENIZER_HPP SHELLLIKETOKENIZER_HPP

/* base class */
#include <QMap>

/* system headers */

/* Qt headers */
#include <QStringList>

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */

/* forward declaration of local classes */


/*!
  \addtogroup Common

  @{
*/

/*!
 \brief class to split up a string respecting quotes, double-quotes and
 backslashes, just like a shell would

*/
class ShellLikeTokenizer
{
public:
   /*!
    \brief default constructor

   */
   ShellLikeTokenizer();

   /*!
    \brief constructor setting input string

    \param input
   */
   ShellLikeTokenizer( const QString &input );

   /*!
    \brief copy constructor

    \param that
   */
   ShellLikeTokenizer( const ShellLikeTokenizer &that );

   /*!
    \brief assignment operator

    \param that
    \return ShellLikeTokenizer &operator
   */
   ShellLikeTokenizer &operator=( const ShellLikeTokenizer &that );

   /*!
    \brief destructor

   */
   virtual ~ShellLikeTokenizer();

   /*!
    \brief set input data

    \param input
   */
   void setInput( const QString &input );
   /*!
    \brief get input data

    \return QString
   */
   QString input();

   /*!
    \brief is input data convertable

    \return bool
   */
   bool isValid();
   /*!
    \brief get converted data

    \return QStringList
   */
   QStringList tokenized();

private:
   /*!
    \brief internal function for splitting

   */
   void split();

   bool           mIsDirty; /*!< \brief flag to show if data needs to be processed */
   bool           mIsValid; /*!< \brief flag to show if last processing was successful */
   QString        mInput;   /*!< \brief input data for processing */
   QStringList    mOutput;  /*!< \brief processed data */
};

/*! @} */

#endif
