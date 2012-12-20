/*
 * \file src/tests/TestAppWebServer/Application.cpp
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

/* local headers */
#include "MainWindow.hpp"


int main(int argc, char *argv[])
{
   int retval = 0;

   QApplication app(argc, argv);
   app.setOrganizationName("SLART");
   app.setOrganizationDomain("svolli.org");
   app.setApplicationName("TestAppIconView");

   MainWindow window;
   window.show();

   retval = app.exec();

   return retval;
}
