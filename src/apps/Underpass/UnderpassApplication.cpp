/*
 * src/apps/Underpass/UnderpassApplication.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* system headers */

/* Qt headers */
#include <QApplication>

/* local library headers */
#include <CrashCleanup.hpp>
#include <GenericSatelliteHandler.hpp>
#include <MainWindow.hpp>
#include <Settings.hpp>
#include <Satellite.hpp>
#include <SorcererLoader.hpp>
#include <Trace.hpp>
#include <Translate.hpp>

/* local headers */
#include "UnderpassMainWidget.hpp"


int main(int argc, char *argv[])
{
   int retval = 0;

   QApplication::setOrganizationName("SLART");
   QApplication::setOrganizationDomain("svolli.org");
   QApplication::setApplicationName("Underpass");

   enableCore();

   QApplication app( argc, argv );
   Settings::setApplicationStyleSheet( &app );

   Translate translate;
   translate.install( &app );

   SorcererLoader::detect();

   GenericSatelliteHandler::createSatellite();

   MainWindow window;
   UnderpassMainWidget *mainWidget = new UnderpassMainWidget( &window );
   window.setMainWidget( mainWidget );
   window.show();

   retval = app.exec();

   Satellite::destroy();

   return retval;
}
