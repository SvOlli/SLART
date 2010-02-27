/**
 * src/apps/Notorious/Application.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* system headers */

/* Qt headers */
#include <QtGui>

/* local library headers */
#include <MainWindow.hpp>
#include <MySettings.hpp>
#include <SorcererLoader.hpp>

/* local headers */
#include "MainWidget.hpp"


int main(int argc, char *argv[])
{
   int retval = 0;

   QApplication app(argc, argv);
   app.setOrganizationName("SLART");
   app.setOrganizationDomain("svolli.org");
   app.setApplicationName("Notorious");
   MySettings settings;

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
   window.setMainWidget( mainWidget );
   window.show();
   
   retval = app.exec();

   return retval;
}
