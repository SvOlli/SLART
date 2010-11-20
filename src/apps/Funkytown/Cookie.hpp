/**
 * src/apps/Funkytown/Cookie.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef COOKIE_HPP
#define COOKIE_HPP COOKIE_HPP

/* base class */

/* system headers */

/* Qt headers */
#include <QString>

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */

/* forward declaration of local classes */


class Cookie
{
   friend class UnitTest;

public:
   Cookie( const QString domain = QString(),
           const QString path = QString(),
           const QString data = QString() );
   virtual ~Cookie();
   Cookie( const Cookie &that );
   Cookie &operator=( const Cookie &that );

   bool matches( const Cookie &that ) const;
   QString getData() const;

private:
   QString mDomain;
   QString mPath;
   QString mData;

};

#endif
