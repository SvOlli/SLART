/*
 * src/apps/Sorcerer/Application.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* system headers */

/* Qt headers */
#include <QApplication>

/* local library headers */
#include <DatabaseInterface.hpp>
#include <GenericSatelliteHandler.hpp>
#include <MainWindow.hpp>
#include <Satellite.hpp>
#include <Settings.hpp>
#include <Trace.hpp>
#include <Translate.hpp>

/* local headers */
#include "SorcererMainWidget.hpp"


int main(int argc, char *argv[])
{
   int retval = 0;

   QApplication::setOrganizationName("SLART");
   QApplication::setOrganizationDomain("svolli.org");
   QApplication::setApplicationName("Sorcerer");

   enableCore();

   QApplication app( argc, argv );
   Q_INIT_RESOURCE( Common );

   Translate translate;
   translate.install( &app );
   translate.load( &app, "Innuendo" );
   translate.load( &app, "Rubberbandman" );
   Settings::setApplicationStyleSheet( &app );
   GenericSatelliteHandler::createSatellite();
   DatabaseInterface::create();

   MainWindow window;
   SorcererMainWidget *mainWidget = new SorcererMainWidget( &window );
   window.setMainWidget( mainWidget );
   window.show();

   retval = app.exec();

   DatabaseInterface::destroy();
   Satellite::destroy();

   return retval | mainWidget->errors();
}
