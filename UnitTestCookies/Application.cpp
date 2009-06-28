/**
 * Application.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */


#include <QtCore>

#include "UnitTest.hpp"


int main(int argc, char *argv[])
{
   int retval = 0;

   QCoreApplication app(argc, argv);
   app.setOrganizationName("SLART");
   app.setOrganizationDomain("svolli.org");
   app.setApplicationName("UnitTestCookies");
   
   UnitTest unitTest;
   unitTest.CookieTest();
   unitTest.CookieJarTest();
   
   return retval;
}
