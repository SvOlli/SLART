/**
 * Application.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#include "MainWindow.hpp"
#include "MainWidget.hpp"
#include "MySettings.hpp"

#include <QtGui>


int main(int argc, char *argv[])
{
   int retval = 0;
   
   QApplication app(argc, argv);
   app.setOrganizationName("SLART");
   app.setOrganizationDomain("svolli.org");
   app.setApplicationName("Sorcerer");
   
   MainWindow window;
   window.show();
   
   retval = app.exec();
   
   return retval;
}
