/*
 * src/apps/Innuendo/Application.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* system headers */

/* Qt headers */
#include <QtGui>

/* local library headers */
#include <GenericSatelliteHandler.hpp>
#include <MainWindow.hpp>
#include <Settings.hpp>
#include <Satellite.hpp>
#include <SorcererLoader.hpp>
#include <Trace.hpp>
#include <Translate.hpp>

/* local headers */
#include "InnuendoMainWidget.hpp"


int main(int argc, char *argv[])
{
   int retval = 0;

   QApplication::setOrganizationName("SLART");
   QApplication::setOrganizationDomain("svolli.org");
   QApplication::setApplicationName("Innuendo");

   enableCore();

   if( argc == 1 )
   {
      QApplication app( argc, argv );
      Q_INIT_RESOURCE( Common );
      Settings::setApplicationStyleSheet( &app );

      Translate translate;
      translate.install( &app );

      SorcererLoader::detect();

      GenericSatelliteHandler::createSatellite();

      MainWindow window;
      InnuendoMainWidget *mainWidget = new InnuendoMainWidget( &window );
      window.setMainWidget( mainWidget );
      window.show();

      retval = app.exec();

      Satellite::destroy();
   }
   else
   {
      QCoreApplication app( argc, argv );

      QStringList message;
      for( int i = 1; i < argc; i++ )
      {
         message.append( argv[i] );
      }
      QObject *msg = GenericSatelliteHandler::send( message.join("\n").toUtf8() );
      QObject::connect( msg, SIGNAL(done()),
                        &app, SLOT(quit()) );
      retval = app.exec();
   }

   return retval;
}
