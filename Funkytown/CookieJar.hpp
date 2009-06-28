/**
 * CookieJar.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#ifndef COOKIEJAR_HPP
#define COOKIEJAR_HPP COOKIEJAR_HPP

#include <QStringList>

#include "Cookie.hpp"

class QString;

class CookieJar
{
   friend class UnitTest;
   
public:
   CookieJar();
   virtual ~CookieJar();

   void store( const QString &line );
   QStringList take( const QString &host, const QString &path );

private:
   QList<Cookie>        mTheJar;
};

#endif
