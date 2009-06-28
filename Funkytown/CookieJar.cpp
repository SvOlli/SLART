/**
 * CookieJar.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#include "CookieJar.hpp"

#include <QStringList>

#include "Trace.hpp"


CookieJar::CookieJar()
: mTheJar()
{
}


CookieJar::~CookieJar()
{
}


void CookieJar::store( const QString &line )
{
   int i;
   
   /* parse the line */
   QStringList components( line.split(';') );
   QString domain;
   QString path;
   
   if( components.size() == 0 )
   {
      return;
   }
   
   for( i = 1; i < components.size(); i++ )
   {
      QString component( components.at(i).trimmed() );
      
      if( component.startsWith( "domain=", Qt::CaseInsensitive ) )
      {
         domain = component.mid( component.indexOf('=') + 1 );
      }
      
      if( component.startsWith( "path=", Qt::CaseInsensitive ) )
      {
         path = component.mid( component.indexOf('=') + 1 );
      }
   }
   
   Cookie cookie( domain, path, components.at(0) );
   
   /* delete an old matching cookie before inserting the new one */
   for( i = 0; i < mTheJar.size(); i++ )
   {
      if( mTheJar.at(i).matches( cookie ) )
      {
         mTheJar.takeAt(i);
         break;
      }
   }
   mTheJar.append( cookie );
}


QStringList CookieJar::take( const QString &host, const QString &path )
{
   int i;
   
   QStringList results;
   Cookie needle( QString(".")+host, path );
   
   for( i = 0; i < mTheJar.size(); i++ )
   {
      if( mTheJar.at(i).matches( needle ) )
      {
         results.append( mTheJar.at(i).getData() );
      }
   }
   
   return results;
}
