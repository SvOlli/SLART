/**
 * Application.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#include "MainWindow.hpp"
#include "MainWidget.hpp"

//#include <unistd.h>
#include <QtGui>


int main(int argc, char *argv[])
{
   int retval = 0;

   QApplication app(argc, argv);
   app.setOrganizationName("SLAT");
   app.setOrganizationDomain("svolli.org");
   app.setApplicationName("TemplateApp");

   MainWindow window;
   window.show();
   
   retval = app.exec();

   return retval;
}
