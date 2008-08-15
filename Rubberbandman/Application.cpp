/**
 * Application.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#include <unistd.h>

#include <QtGui>
#include "MainWindow.hpp"
#include "MySettings.hpp"
#include "Database.hpp"


int main(int argc, char *argv[])
{
   int retval = 0;

   QApplication app(argc, argv);
   app.setOrganizationName("SLART");
   app.setOrganizationDomain("svolli.org");
   app.setApplicationName("Rubberbandman");
   
   if( !MySettings().contains( "SLARTCommunication" ) || !Database::exists() )
   {
      if( !MainWindow::invokeSetUp( argv[0] ) )
      {
         QMessageBox::critical( 0, app.applicationName(), QObject::tr("Setup failed!\nCannot start.\nSorry.") );
         return 1;
      }
   }
   
   MainWindow window;
   window.show();
   retval = app.exec();
   
   return retval;
}
