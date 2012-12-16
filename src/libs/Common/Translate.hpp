/* * src/libs/Common/Translate.hpp
 * written by Sven Oliver Moll
*
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef TRANSLATE_HPP
#define TRANSLATE_HPP TRANSLATE_HPP

/* system headers */

/* Qt headers */
#include <QList>

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */
class QCoreApplication;
class QString;
class QTranslator;

/* forward declaration of local classes */


/*!
  \addtogroup Common

  @{
*/

/*!
 \brief wrapper for handling multiple translation sources

*/
class Translate
{
public:
   Translate();
   virtual ~Translate();

   /*!
    \brief install the default translators

    \param app application to add translators to ( 0 = qApp )
   */
   void install( QCoreApplication *app = 0 );

   /*!
    \brief install an additional translator

    \param app application to add translator
    \param catalog base name of .ts file to install
   */
   void load( QCoreApplication *app, const QString &catalog );

private:
   Q_DISABLE_COPY( Translate )

   /*!
    \brief get the location of own translation files

   */
   static QString location( QCoreApplication *app );

   QList<QTranslator*>  mTranslators;
};

/*! @} */

#endif
