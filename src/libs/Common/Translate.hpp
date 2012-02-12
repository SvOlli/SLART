/*
 * src/libs/Common/Translate.hpp
 * written by Sven Oliver Moll
*
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef TRANSLATE_HPP
#define TRANSLATE_HPP TRANSLATE_HPP

/* system headers */

/* Qt headers */
#include <QTranslator>

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */
class QCoreApplication;
class QString;

/* forward declaration of local classes */


/*!
  \addtogroup Common

  @{
*/

/*!
 \brief \todo complete documentation

*/
class Translate
{
public:
   Translate();
   virtual ~Translate();

   /*!
    \brief install the translators

   */
   void install( QCoreApplication *app = 0 );

private:
   Q_DISABLE_COPY( Translate )

   /*!
    \brief get the location of own translation files

   */
   static QString location( QCoreApplication *app );

   QTranslator    *mpQtTranslator;
   QTranslator    *mpLibTranslator;
   QTranslator    *mpAppTranslator;
};

/*! @} */

#endif
