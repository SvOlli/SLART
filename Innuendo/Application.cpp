/**
 * Application.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#include "MainWindow.hpp"
#include "MainWidget.hpp"
#include "MySettings.hpp"
#if MAINWINDOW_SORCERER
#include "Database.hpp"
#endif

#include <QtGui>


int main(int argc, char *argv[])
{
   int retval = 0;

   QApplication::setOrganizationName("SLART");
   QApplication::setOrganizationDomain("svolli.org");
   QApplication::setApplicationName("Innuendo");
   
   if( argc == 1 )
   {
      MySettings settings;
      QApplication app(argc, argv);
   
#if MAINWINDOW_SORCERER
      if( !settings.contains( "SLARTCommunication" ) || !Database::exists() )
      {
         if( !MainWindow::invokeSetUp( &app ) )
         {
            return 2;
         }
      }
#endif
      {
         QFile qssFile( settings.styleSheetFile() );
         if( qssFile.exists() && qssFile.open( QIODevice::ReadOnly ) )
         {
            app.setStyleSheet( qssFile.readAll() );
            qssFile.close();
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
