/**
 * Application.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#include "EventHandler.hpp"

#include <QtGui>


int main(int argc, char *argv[])
{
   int retval = 0;

   QApplication app(argc, argv);
   app.setOrganizationName("SLART");
   app.setOrganizationDomain("svolli.org");
   app.setApplicationName("Partyman"); // lying here, so new settings are necessary
   
   EventHandler eventHandler;
   eventHandler.showMinimized();
   
   retval = app.exec();

   return retval;
}
