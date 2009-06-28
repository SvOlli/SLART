/**
 * Cookie.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#include "Cookie.hpp"

#include "Trace.hpp"


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


Cookie::Cookie( const Cookie &other )
: mDomain( other.mDomain )
, mPath( other.mPath )
, mData( other.mData )
{
}


Cookie &Cookie::operator=( const Cookie &other )
{
   mDomain = other.mDomain;
   mPath   = other.mPath;
   mData   = other.mData;
   
   return *this;
}


bool Cookie::matches( const Cookie &other ) const
{
   if( other.mDomain.right( mDomain.size() ) != mDomain )
   {
      return false;
   }
   
   if( other.mPath.left( mPath.size() ) != mPath )
   {
      return false;
   }
   
   /* no data is a wildcard that matches all */
   if( other.mData.size() == 0 )
   {
      return true;
   }
   
   return other.mData.left( other.mData.indexOf('=') ) 
           == mData.left( mData.indexOf('=') );
}

QString Cookie::getData() const
{
   return mData;
}

