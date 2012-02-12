/*
 * src/apps/Funkytown/CookieJar.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef COOKIEJAR_HPP
#define COOKIEJAR_HPP COOKIEJAR_HPP

/* base class */

/* system headers */

/* Qt headers */
#include <QStringList>

/* local library headers */

/* local headers */
#include "Cookie.hpp"

/* forward declaration of Qt classes */
class QString;

/* forward declaration of local classes */


/*!
  \addtogroup Funkytown

  @{
*/

/*!
 \brief \todo complete documentation

*/
class CookieJar
{
   friend class UnitTest;

public:
   CookieJar();
   virtual ~CookieJar();

   /*!
    \brief store a cookie

   */
   void store( const QString &line );
   /*!
    \brief retrieve a cookie

   */
   QStringList take( const QString &host, const QString &path );

private:
   QList<Cookie>        mTheJar;
};

/*! @} */

#endif
