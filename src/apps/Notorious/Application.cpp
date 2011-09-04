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
#include <Trace.hpp>
#include <Translate.hpp>

/* local headers */
#include "MainWidget.hpp"


int main(int argc, char *argv[])
{
   int retval = 0;

   QApplication::setOrganizationName("SLART");
   QApplication::setOrganizationDomain("svolli.org");
   QApplication::setApplicationName("Notorious");

   enableCore();

   QApplication app( argc, argv );

   Translate translate;
   translate.install( &app );

   MySettings settings;
   //SorcererLoader::detect();
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
