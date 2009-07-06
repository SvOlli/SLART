/**
 * Application.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#include "MainWindow.hpp"
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
   QApplication::setApplicationName("Partyman");
   
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
      window.mainWidget()->startUp();
      
      retval = app.exec();
   }
   
   return retval;
}
