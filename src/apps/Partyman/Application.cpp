/**
 * Application.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#include <QtGui>
#include <stdlib.h>

#include "MainWidget.hpp"
#include "MainWindow.hpp"
#include "MySettings.hpp"
#include "SorcererLoader.hpp"


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
      while( args.size() > 0 )
      {
         settings.sendUdpMessage( QFileInfo( args.takeFirst() ).absoluteFilePath().prepend( "P0Q\n" ) );
      }
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
         
         MainWindow window;
         MainWidget *mainWidget = new MainWidget( &window );
         window.show();
         mainWidget->startUp();
         
         retval = app.exec();
      }
   }
   
   return retval;
}
