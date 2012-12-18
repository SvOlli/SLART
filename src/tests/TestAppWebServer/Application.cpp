/*
 * src/tests/TestAppWebServer/Application.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

/* system headers */

/* Qt headers */
#include <QtGui>

/* local library headers */
#include <GenericSatelliteHandler.hpp>
#include <MainWindow.hpp>
#include <Satellite.hpp>

/* local headers */
#include "MainWidget.hpp"
#include "WebServerSanityTests.hpp"

int main(int argc, char *argv[])
{
   int retval = 0;

   QApplication app(argc, argv);
   app.setOrganizationName("SLART");
   app.setOrganizationDomain("svolli.org");
   app.setApplicationName("TestAppWebServer");

   WebServerSanityTests::scgiRequest();

   GenericSatelliteHandler::createSatellite();

   MainWindow window;
   MainWidget *mainWidget = new MainWidget( &window );
   window.setMainWidget( mainWidget );
   window.show();

   retval = app.exec();

   Satellite::destroy();

   return retval;
}
