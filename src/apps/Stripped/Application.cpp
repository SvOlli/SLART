/**
 * src/apps/Stripped/Application.cpp
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
#include <Satellite.hpp>
#include <Trace.hpp>
#include <Translate.hpp>

/* local headers */
#include "MainWidget.hpp"


int main(int argc, char *argv[])
{
   int retval = 0;

   QApplication::setOrganizationName("SLART");
   QApplication::setOrganizationDomain("svolli.org");
   QApplication::setApplicationName("Stripped");

   enableCore();

   QApplication app( argc, argv );

   Translate translate;
   translate.install( &app );

   Satellite::create();

   MySettings settings;

#if MAINWINDOW_SORCERER
   if( !MySettings().contains( "SLARTCommunication" ) || !Database::exists() )
   {
      if( !MainWindow::invokeSetUp( &app ) )
      {
         QMessageBox::critical( 0, app.applicationName(), QObject::tr("Setup failed!\nCannot start.\nSorry.") );
         return 1;
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
   MainWidget *mainWidget = new MainWidget( &window );
   window.setMainWidget( mainWidget );
   window.show();

   retval = app.exec();

   Satellite::destroy();

   return retval;
}
