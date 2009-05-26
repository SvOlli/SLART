/**
 * Application.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#include "MainWindow.hpp"
#include "MainWidget.hpp"
#include "MySettings.hpp"

#include <QtGui>


int main(int argc, char *argv[])
{
   int retval = 0;

   QApplication::setOrganizationName("SLART");
   QApplication::setOrganizationDomain("svolli.org");
   QApplication::setApplicationName("Innuendo");
   
   if( argc == 1 )
   {
      QApplication app(argc, argv);
      
      if( !MySettings().contains( "SLARTCommunication" ) )
      {
         if( !MainWindow::invokeSetUp( &app ) )
         {
            return 2;
         }
      }
      
      MainWindow window;
      window.show();
      
      retval = app.exec();
   }
   else if( argc > 2 )
   {
      QCoreApplication app(argc, argv);
      
      QString application( argv[1] );
      QStringList message;
      for( int i = 2; i < argc; i++ )
      {
         message.append( argv[i] );
      }
      MySettings().sendUdpMessage( message.join("\n"), application );
   }
   else
   {
      fprintf( stderr, "Usage: %s (<application> <message>)\n", argv[0] );
      retval = 1;
   }
   
   return retval;
}
