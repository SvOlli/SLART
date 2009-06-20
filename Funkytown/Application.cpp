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
#include "MySettings.hpp"
#include "ConfigDialog.hpp"
#if MAINWINDOW_SORCERER
#include "Database.hpp"
#endif

#include "Trace.hpp"

int main(int argc, char *argv[])
{
   int retval = 0;
   
   QApplication app(argc, argv);
   app.setOrganizationName("SLART");
   app.setOrganizationDomain("svolli.org");
   app.setApplicationName("Funkytown");
   QStringList *startUrls = new QStringList();
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
      QFile qssFile( settings.VALUE_STYLESHEET );
      if( qssFile.open( QIODevice::ReadOnly ) )
      {
         app.setStyleSheet( qssFile.readAll() );
         qssFile.close();
      }
   }
   
   for( retval = 1; retval < argc; retval++ )
   {
      QFileInfo qfi( argv[retval] );
      if( QFileInfo( argv[retval] ).isFile() )
      {
         QString line;
         QFile input( argv[retval] );
         if( !input.open( QIODevice::ReadOnly | QIODevice::Text ) )
         {
            continue;
         }
         while( !input.atEnd() )
         {
            line = QString::fromLocal8Bit( input.readLine() );
            if( line.startsWith("http") )
            {
               *startUrls << line.trimmed();
            }
         }
         input.close();
      }
      else
      {
         *startUrls << QString::fromLocal8Bit( argv[retval] ).trimmed();
      }
   }
   
   MainWindow window;
   for( retval = 0; retval < startUrls->size(); retval++ )
   {
      window.mainWidget()->downloadUserPage( startUrls->at(retval) );
   }
   delete startUrls;
   window.show();
   retval = app.exec();
   
   return retval;
}
