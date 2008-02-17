/**
 * Application.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#include <unistd.h>

#include <QApplication>
#include "MainWindow.hpp"


int main(int argc, char *argv[])
{
   int retval = 0;

   QApplication app(argc, argv);
   app.setOrganizationName("SLAT");
   app.setOrganizationDomain("svolli.org");
   app.setApplicationName("Rubberbandman");
   
   MainWindow window;
   window.show();
   retval = app.exec();
   window.shutdown();
   
   return retval;
}
