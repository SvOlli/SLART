/*
 * \file src/tests/TestAppDragAndDrop/Application.cpp
 * \author written by Sven Oliver Moll
 *
 * \copyright
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

/* system headers */

/* Qt headers */
#include <QApplication>

/* local library headers */
#include <MainWindow.hpp>

/* local headers */
#include "MainWidget.hpp"


int main(int argc, char *argv[])
{
   int retval = 0;

   QApplication app(argc, argv);
   app.setOrganizationName("SLART");
   app.setOrganizationDomain("svolli.org");
   app.setApplicationName("TestAppDragAndDrop");

   MainWindow window;
   MainWidget *mainWidget = new MainWidget( &window );
   window.setMainWidget( mainWidget );
   window.show();

   retval = app.exec();

   return retval;
}
