/**
 * src/apps/Rubberbandman/Application.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* system headers */
#include <cstdlib>

/* Qt headers */
#include <QtGui>
#include <QTimer>

/* local library headers */
#include <Database.hpp>
#include <MainWindow.hpp>
#include <MySettings.hpp>
#include <SorcererLoader.hpp>

/* local headers */
#include "ConfigDialog.hpp"
#include "Console.hpp"
#include "DatabaseWorker.hpp"
#include "MainWidget.hpp"


int main(int argc, char *argv[])
{
   int retval = 0;

   QApplication::setOrganizationName("SLART");
   QApplication::setOrganizationDomain("svolli.org");
   QApplication::setApplicationName("Rubberbandman");

#ifdef Q_WS_X11
   bool useGUI = getenv("DISPLAY") != 0;
#else
   bool useGUI = true;
#endif
   QApplication app(argc, argv, useGUI);

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
            Console console( QObject::tr( "entries checked" ),
                             QObject::tr( "cleaned" ) );
            QObject::connect( databaseWorker, SIGNAL(progress(int,int)),
                              &console, SLOT(handleProgress(int,int)) );
            QObject::connect( databaseWorker, SIGNAL(finished()),
                              qApp, SLOT(quit()) );
            QTimer::singleShot( 1, databaseWorker, SLOT(start()) );
            app.exec();
         }
         else if( arg == _update )
         {
            QString baseDir( MySettings( "Global" ).VALUE_MUSICBASE );
            if( !baseDir.isEmpty() )
            {
               databaseWorker->initUpdate( baseDir );
               Console console( QObject::tr( "files scanned" ),
                                QObject::tr( "updated" ) );
               QObject::connect( databaseWorker, SIGNAL(progress(int,int)),
                                 &console, SLOT(handleProgress(int,int)) );
               QObject::connect( databaseWorker, SIGNAL(finished()),
                                 qApp, SLOT(quit()) );
               QTimer::singleShot( 1, databaseWorker, SLOT(start()) );
               app.exec();
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
      if( useGUI )
      {
         SorcererLoader::detect( &app );

         {
            QFile qssFile( MySettings().styleSheetFile() );
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
      }
   }

   return retval;
}
