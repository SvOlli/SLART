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

#ifndef VALUE_STYLESHEET
#define VALUE_STYLESHEET value("StyleSheet", QString()).toString()
#endif

#include <QtGui>


int main(int argc, char *argv[])
{
   int retval = 0;

   QApplication app(argc, argv);
   app.setOrganizationName("SLART");
   app.setOrganizationDomain("svolli.org");
   app.setApplicationName("TestAppGlobalShortcut");
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
   window.show();
   
   retval = app.exec();

   return retval;
}
