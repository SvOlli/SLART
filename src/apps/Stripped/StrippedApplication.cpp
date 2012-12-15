/*
 * src/apps/Stripped/StrippedApplication.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* system headers */

/* Qt headers */
#include <QtGui>

/* local library headers */
#include <GenericSatMsgHandler.hpp>
#include <Satellite.hpp>
#include <Settings.hpp>
#include <SingleInstance.hpp>
#include <Trace.hpp>
#include <Translate.hpp>

/* local headers */
#include "StrippedConfigDialog.hpp"
#include "StrippedMainWindow.hpp"


int main(int argc, char *argv[])
{
   int retval = 0;

   QApplication::setOrganizationName("SLART");
   QApplication::setOrganizationDomain("svolli.org");
   QApplication::setApplicationName("Stripped");

   enableCore();

   QApplication app( argc, argv );

   Translate translate;
   translate.install( &app );
   Settings::setApplicationStyleSheet( &app );
   Satellite *s = GenericSatMsgHandler::createSatellite();

   StrippedMainWindow window;
   window.show();

   if( s ) s->start();
   retval = app.exec();

   Satellite::destroy();

   return retval;
}
