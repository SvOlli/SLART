/**
 * UnitTest.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */


#include "UnitTest.hpp"

#include "../Funkytown/CookieJar.hpp"

#include "Trace.hpp"


UnitTest::UnitTest()
{
}


UnitTest::~UnitTest()
{
}


bool UnitTest::CookieTest()
{
TRACESTART(UnitTest::CookieTest)
   bool success( true );
   Cookie c1(".svolli.xa","/","COOKIE1=TestData1");
   Cookie c2(".svolli.xa","/","COOKIE2=TestData2");
   Cookie c3(".svolli.xa","/");
   Cookie c4(".svolli.xx","/");
   
   if( !c1.matches( c1 ) )
   {
TRACECP
      success = false;
   }
   
   if( !c2.matches( c2 ) )
   {
TRACECP
      success = false;
   }
   
   if( c1.matches( c2 ) )
   {
TRACECP
      success = false;
   }
   
   if( !c1.matches( c3 ) )
   {
TRACECP
      success = false;
   }
   
   if( !c1.matches( c3 ) )
   {
TRACECP
      success = false;
   }
   
   if( c1.matches( c4 ) )
   {
TRACECP
      success = false;
   }
   
   return success;
}

bool UnitTest::CookieJarTest()
{
TRACESTART(UnitTest::CookieJarTest)
   bool success( true );
   
   CookieJar cj;
   cj.store( "COOKIE1=TestData1; domain=.svolli.xa; expires=Tue, 30-Jun-2009 21:23:58 GMT; path=/" );
   cj.store( "COOKIE2=TestData1; domain=.svolli.xa; expires=Tue, 30-Jun-2009 21:23:58 GMT; path=/" );

TRACEMSG << cj.take( "www.svolli.xa", "/" );
TRACEMSG << cj.take( "svolli.xa", "/" );
   
   
   return success;
}
