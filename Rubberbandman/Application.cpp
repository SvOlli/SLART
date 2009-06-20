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
#if MAINWINDOW_SORCERER
#include "Database.hpp"
#endif
#include "DatabaseWorker.hpp"
#include "ConfigDialog.hpp"


int main(int argc, char *argv[])
{
   int retval = 0;

   QApplication::setOrganizationName("SLART");
   QApplication::setOrganizationDomain("svolli.org");
   QApplication::setApplicationName("Rubberbandman");
   
   if( (argc > 1) && strcmp(argv[1],"-qws") )
   {
      QCoreApplication app(argc, argv);
      bool fail    = false;
      bool cleanup = false;
      bool update  = false;
      int opt;
      
      if( !Database::exists() )
      {
         return 2;
      }
      
      while ((opt = getopt(argc, argv, "hcu")) != -1)
      {
         switch(opt)
         {
            case 'c':
               cleanup = true;
               break;
            case 'u':
               update = true;
               break;
            case 'h':
            default:
               fail = true;
         }
      }
      
      if( fail )
      {
         fprintf( stderr, "Usage:\t%s\n"
                  "cleanup:%s -c\n"
                  "update:\t%s -u\n",
                  argv[0], argv[0], argv[0] );
         return 1;
      }
      
      Database db;
      DatabaseWorker *databaseWorker = new DatabaseWorker();
      databaseWorker->prepare( &db );
      
      if( cleanup )
      {
         databaseWorker->initCleanup();
         databaseWorker->start();
         databaseWorker->wait();
      }
      
      if( update )
      {
         QString baseDir( MySettings( "Global" ).VALUE_MUSICBASE );
         if( !baseDir.isEmpty() )
         {
            databaseWorker->initUpdate( baseDir );
            databaseWorker->start();
            databaseWorker->wait();
         }
      }
   }
   else
   {
      MySettings settings;
      QApplication app(argc, argv);
   
      if( !settings.contains( "SLARTCommunication" ) || !Database::exists() )
      {
         if( !MainWindow::invokeSetUp( &app ) )
         {
            return 2;
         }
      }
      
      {
         QFile qssFile( settings.VALUE_STYLESHEET );
         if( qssFile.open( QIODevice::ReadOnly ) )
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
