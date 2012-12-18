/*
 * src/tests/TestAppSatellite/Application.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

/* system headers */

/* Qt headers */
#include <QtGui>
#include <QtNetwork>

/* local library headers */
#include <Satellite.hpp>

/* local headers */
#include "MainWindow.hpp"
#include "MainWidget.hpp"



int main(int argc, char *argv[])
{
   int retval = 0;

   QApplication app(argc, argv);
   app.setOrganizationName("SLART");
   app.setOrganizationDomain("svolli.org");
   app.setApplicationName("TestAppSatellite");

   Satellite::create( QHostAddress::LocalHost, 24224 );

   MainWindow window;
   window.show();

   retval = app.exec();

   Satellite::destroy();

   return retval;
}
