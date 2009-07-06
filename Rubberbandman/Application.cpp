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
#include "ConfigDialog.hpp"
#include "DatabaseWorker.hpp"

#include <QtGui>


int main(int argc, char *argv[])
{
   int retval = 0;
   
   QApplication::setOrganizationName("SLART");
   QApplication::setOrganizationDomain("svolli.org");
   QApplication::setApplicationName("Rubberbandman");
   
   QApplication app(argc, argv);
   
   QStringList args( QApplication::arguments() );
   if( args.size() > 1 )
   {
      args.takeFirst(); // first argument is program name
      const QString _cleanup( "-cleanup" );
      const QString _update( "-update" );
      
      if( !Database::exists() )
      {
         return 2;
      }
      
      QString arg;
      Database db;
      DatabaseWorker *databaseWorker = new DatabaseWorker();
      databaseWorker->prepare( &db );
      while( args.size() > 0 )
      {
         arg = args.takeFirst();
         if( arg == _cleanup )
         {
            databaseWorker->initCleanup();
            databaseWorker->start();
            databaseWorker->wait();
         }
         else if( arg == _update )
         {
            QString baseDir( MySettings( "Global" ).VALUE_MUSICBASE );
            if( !baseDir.isEmpty() )
            {
               databaseWorker->initUpdate( baseDir );
               databaseWorker->start();
               databaseWorker->wait();
            }
         }
         else
         {
            fprintf( stderr, "%s: (%s) (%s)\n", 
                     argv[0], _cleanup.toLocal8Bit().constData(), _update.toLocal8Bit().constData() );
            return 1;
         }
      }
   }
   else
   {
      MySettings settings;
   
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
   
   return retval;
}
