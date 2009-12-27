/**
 * src/apps/Funkytown/Cookie.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef COOKIE_HPP
#define COOKIE_HPP COOKIE_HPP

#include <QString>

class Cookie
{
   friend class UnitTest;
   
public:
   Cookie( const QString domain = QString(),
           const QString path = QString(),
           const QString data = QString() );
   virtual ~Cookie();
   Cookie( const Cookie &other );
   Cookie &operator=( const Cookie &other );
   
   bool matches( const Cookie &other ) const;
   QString getData() const;
   
private:
   QString mDomain;
   QString mPath;
   QString mData;
   
};

#endif
