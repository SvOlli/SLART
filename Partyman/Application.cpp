/**
 * Application.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#include "MainWindow.hpp"
#include "MainWidget.hpp"
#include "MySettings.hpp"
#include "Database.hpp"

#include <QtGui>


int main(int argc, char *argv[])
{
   int retval = 0;
   
   QApplication app(argc, argv);
   app.setOrganizationName("SLART");
   app.setOrganizationDomain("svolli.org");
   app.setApplicationName("Partyman");
   
   if( !MySettings().contains( "SLARTCommunication" ) || !Database::exists() )
   {
      if( !MainWindow::invokeSetUp( argv[0] ) )
      {
         QMessageBox::critical( 0, app.applicationName(), QObject::tr("Setup failed!\nCannot start.\nSorry.") );
         return 1;
      }
   }
   
   if( argc > 1 )
   {
      MySettings settings;
      for( int i = 1; i < argc; i++ )
      {
         settings.sendUdpMessage( QFileInfo( QString::fromLocal8Bit(argv[i]) ).absoluteFilePath().prepend( "P0Q\n" ) );
      }
      
      return 0;
   }

   MainWindow window;
   window.show();
   window.mainWidget()->startUp();
      
   retval = app.exec();

   return retval;
}
