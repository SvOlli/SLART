/*
 * src/apps/Rubberbandman/RubberbandmanApplication.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* system headers */
#include <cstdlib>

/* Qt headers */
#include <QApplication>
#include <QTimer>

/* local library headers */
#include <DatabaseInterface.hpp>
#include <GenericSatelliteHandler.hpp>
#include <MainWindow.hpp>
#include <Satellite.hpp>
#include <Settings.hpp>
#include <SorcererLoader.hpp>
#include <ThreadAutoStart.hpp>
#include <Trace.hpp>
#include <Translate.hpp>

/* local headers */
#include "Console.hpp"
#include "DatabaseWorker.hpp"
#include "RubberbandmanCommandLine.hpp"
#include "RubberbandmanMainWidget.hpp"


int main(int argc, char *argv[])
{
   int retval = 0;

   QApplication::setOrganizationName("SLART");
   QApplication::setOrganizationDomain("svolli.org");
   QApplication::setApplicationName("Rubberbandman");

   enableCore();

#ifdef Q_WS_X11
   bool useGUI = getenv("DISPLAY") != 0;
#else
   bool useGUI = true;
#endif
   QApplication app( argc, argv, useGUI );

   Translate *translate = new Translate( &app );
   translate->install();

   DatabaseInterface::create();

   QStringList args( QApplication::arguments() );
   if( args.size() > 1 )
   {
      args.takeFirst(); // first is program name
      QThread *walkerThread = new QThread();
      walkerThread->setObjectName( "WalkerThread" );
      RubberbandmanCommandLine *cmdLine = new RubberbandmanCommandLine( args );
      cmdLine->moveToThread( walkerThread );
      QObject::connect( cmdLine, SIGNAL(done()),
                        qApp, SLOT(quit()) );
      QObject::connect( qApp, SIGNAL(aboutToQuit()),
                        walkerThread, SLOT(quit()) );
      new ThreadAutoStart( walkerThread );
      QTimer::singleShot( 1, cmdLine, SLOT(nextJob()) );
      app.exec();
      delete cmdLine;
   }
   else
   {
      if( useGUI )
      {
         Settings::setApplicationStyleSheet( &app );

         SorcererLoader::detect();

         GenericSatelliteHandler::createSatellite();

         MainWindow window;
         RubberbandmanMainWidget *mainWidget = new RubberbandmanMainWidget( &window );
         window.setMainWidget( mainWidget );
         window.show();

         retval = app.exec();

         Satellite::destroy();
      }
   }

   DatabaseInterface::destroy();

   return retval;
}
