/**
 * Application.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

/* system headers */

/* Qt headers */
#include <QtGui>

/* local library headers */

/* local headers */
#include "MainWindow.hpp"
#include "MainWidget.hpp"



int main(int argc, char *argv[])
{
   int retval = 0;

   QApplication app(argc, argv);
   app.setOrganizationName("SLART");
   app.setOrganizationDomain("svolli.org");
   app.setApplicationName("TestApp");

   MainWindow window;
   window.show();
   
   retval = app.exec();

   return retval;
}
