/**
 * src/apps/Partyman/Application.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* system headers */
#include <cstdlib>

/* Qt headers */
#include <QtGui>

/* local library headers */
#include <MainWindowCheckClose.hpp>
#include <MySettings.hpp>
#include <Satellite.hpp>
#include <SorcererLoader.hpp>

/* local headers */
#include "CommandLineHandler.hpp"
#include "MainWidget.hpp"


int main(int argc, char *argv[])
{
   int retval = 0;

   QApplication::setOrganizationName("SLART");
   QApplication::setOrganizationDomain("svolli.org");
   QApplication::setApplicationName("Partyman");

   MySettings settings;
#ifdef Q_WS_X11
   bool useGUI = getenv("DISPLAY") != 0;
#else
   bool useGUI = true;
#endif
   QApplication app(argc, argv, useGUI);

   QStringList args( QApplication::arguments() );
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
         SorcererLoader::detect( &app );

         {
            QFile qssFile( settings.styleSheetFile() );
            if( qssFile.exists() && qssFile.open( QIODevice::ReadOnly ) )
            {
               app.setStyleSheet( qssFile.readAll() );
               qssFile.close();
            }
         }

         MainWindowCheckClose window;
         MainWidget *mainWidget = new MainWidget( &window );
         window.setMainWidget( mainWidget );
         window.show();
         mainWidget->startUp();

         retval = app.exec();
      }
   }

   return retval;
}
