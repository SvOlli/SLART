/*
 * src/apps/Funkytown/Cookie.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "Cookie.hpp"

/* system headers */

/* Qt headers */

/* local library headers */

/* local headers */


Cookie::Cookie( const QString domain,
                const QString path,
                const QString data )
: mDomain( domain )
, mPath( path )
, mData( data )
{
}


Cookie::~Cookie()
{
}


Cookie::Cookie( const Cookie &that )
: mDomain( that.mDomain )
, mPath( that.mPath )
, mData( that.mData )
{
}


Cookie &Cookie::operator=( const Cookie &that )
{
   if( this == &that )
   {
      return *this;
   }
   mDomain = that.mDomain;
   mPath   = that.mPath;
   mData   = that.mData;

   return *this;
}


bool Cookie::matches( const Cookie &that ) const
{
   if( that.mDomain.right( mDomain.size() ) != mDomain )
   {
      return false;
   }

   if( that.mPath.left( mPath.size() ) != mPath )
   {
      return false;
   }

   /* no data is a wildcard that matches all */
   if( that.mData.size() == 0 )
   {
      return true;
   }

   return that.mData.left( that.mData.indexOf('=') )
           == mData.left( mData.indexOf('=') );
}


QString Cookie::getData() const
{
   return mData;
}

