/**
 * src/tests/TestAppWebServer/Application.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the Lesser GNU Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#include "MainWindow.hpp"
#include "MainWidget.hpp"

#include <QtGui>


int main(int argc, char *argv[])
{
   int retval = 0;

   QApplication app(argc, argv);
   app.setOrganizationName("SLART");
   app.setOrganizationDomain("svolli.org");
   app.setApplicationName("TestAppWebServer");

   MainWindow window;
   MainWidget *mainWidget = new MainWidget( &window );
   window.setMainWidget( mainWidget );
   window.show();
   
   retval = app.exec();
   
   return retval;
}
