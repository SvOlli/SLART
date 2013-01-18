/*
 * src/apps/Partyman/PartymanApplication.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* system headers */
#include <cstdlib>

/* Qt headers */
#include <QtGui>

/* local library headers */
#include <GenericSatelliteHandler.hpp>
#include <MainWindowCheckClose.hpp>
#include <Satellite.hpp>
#include <Settings.hpp>
#include <SorcererLoader.hpp>
#include <Trace.hpp>
#include <Translate.hpp>

/* local headers */
#include "CommandLineHandler.hpp"
#include "PartymanMainWindow.hpp"

/*!
 \addtogroup Partyman

 @{
 \brief

 \param argc
 \param argv

 \dotfile "graphs/apps/Partyman/PartymanApplication_connect.dot" "Connect Graph"

 @}
 */
int main(int argc, char *argv[])
{
   int retval = 0;

   QApplication::setOrganizationName("SLART");
   QApplication::setOrganizationDomain("svolli.org");
   QApplication::setApplicationName("Partyman");

   enableCore();

   bool useGUI = true;
#ifdef Q_WS_X11
   useGUI = getenv("DISPLAY") != 0;
#endif
   QApplication app( argc, argv, useGUI );

   Translate translate;
   translate.install( &app );

   QStringList args( QApplication::arguments() );
   GenericSatelliteHandler::createSatellite();
   if( args.size() > 1 )
   {
      args.takeFirst(); // first argument is program name
      CommandLineHandler commandLineHandler( args );
      retval = app.exec();
   }
   else
   {
      if( useGUI )
      {
         Q_INIT_RESOURCE( Common );
         Settings::setApplicationStyleSheet( &app );
         SorcererLoader::detect();

         PartymanMainWindow window;
         window.show();

         retval = app.exec();
      }
   }

   Satellite::destroy();

   return retval;
}
