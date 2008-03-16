/**
 * Application.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#include <unistd.h>

#include <QApplication>
#include <QFileInfo>

#include "MainWindow.hpp"
#include "MainWidget.hpp"


int main(int argc, char *argv[])
{
   int retval = 0;
   
   QApplication app(argc, argv);
#ifdef STANDALONE
   app.setOrganizationName("MySpace");
   app.setOrganizationDomain("myspace.com");
   app.setApplicationName("Hack");
#else
   app.setOrganizationName("SLART");
   app.setOrganizationDomain("svolli.org");
   app.setApplicationName("Funkytown");
#endif

   MainWindow window;
   for( retval = 1; retval < argc; retval++ )
   {
      if( QFileInfo( argv[retval] ).isFile() )
      {
         // TODO: read file as QStringList
      }
      else
      {
         window.mainWidget()->downloadUserPage( argv[retval] );
      }
   }

   window.show();
   retval = app.exec();
   
   return retval;
}
