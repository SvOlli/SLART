/*
 * src/apps/Rubberbandman/Application.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* system headers */
#include <cstdlib>

/* Qt headers */
#include <QtGui>
#include <QTimer>

/* local library headers */
#include <DatabaseInterface.hpp>
#include <GenericSatMsgHandler.hpp>
#include <MainWindow.hpp>
#include <MySettings.hpp>
#include <Satellite.hpp>
#include <SorcererLoader.hpp>
#include <Trace.hpp>
#include <Translate.hpp>

/* local headers */
#include "ConfigDialog.hpp"
#include "Console.hpp"
#include "DatabaseWorker.hpp"
#include "RubberbandmanMainWidget.hpp"


/*!
 \addtogroup Rubberbandman

 @{
 \brief

 \param argc
 \param argv

 \dotfile "graphs/apps/Rubberbandman/Application_connect.dot" "Connect Graph"
 @}
 */
int main(int argc, char *argv[])
{
   int retval = 0;

   QApplication::setOrganizationName("SLART");
   QApplication::setOrganizationDomain("svolli.org");
   QApplication::setApplicationName("Rubberbandman");

   enableCore();

#ifdef Q_WS_X11
   bool useGUI = getenv("DISPLAY") != 0;
#else
   bool useGUI = true;
#endif
   QApplication app( argc, argv, useGUI );

   Translate translate;
   translate.install( &app );

   DatabaseInterface::create();

   QStringList args( QApplication::arguments() );
   if( args.size() > 1 )
   {
      QTextStream stdErr( ::stderr, QIODevice::WriteOnly );
      args.takeFirst(); // first argument is program name
      const QString _cleanup( "-cleanup" );
      const QString _basedir( "-basedir" );
      const QString _update( "-update" );

      if( !DatabaseInterface::exists() )
      {
         return 2;
      }

      QString arg;
      QString baseDir( MySettings( "Global" ).VALUE_MUSICBASE );
      DatabaseWorker *databaseWorker = new DatabaseWorker();
      while( args.size() > 0 )
      {
         arg = args.takeFirst();
         if( arg == _basedir )
         {
            if( args.size() > 1 )
            {
               baseDir = args.takeFirst();
            }
         }
         else if( arg == _cleanup )
         {
            databaseWorker->startCleanup();
            Console console( QObject::tr( "entries checked" ),
                             QObject::tr( "cleaned" ) );
            QObject::connect( databaseWorker, SIGNAL(progress(int,int)),
                              &console, SLOT(handleProgress(int,int)) );
            QObject::connect( databaseWorker, SIGNAL(finished()),
                              qApp, SLOT(quit()) );
            console.message( QObject::tr("cleaning up") );
            QTimer::singleShot( 0, databaseWorker, SLOT(start()) );
            app.exec();
         }
         else if( arg == _update )
         {
            if( !baseDir.isEmpty() )
            {
               if( !QFileInfo( baseDir ).isDir() )
               {
                  stdErr << QObject::tr( "%1 is not a directory\n" ).arg( baseDir );
               }
               else
               {
                  databaseWorker->startUpdate( baseDir );
                  Console console( QObject::tr( "files scanned" ),
                                   QObject::tr( "updated" ) );
                  QObject::connect( databaseWorker, SIGNAL(progress(int,int)),
                                    &console, SLOT(handleProgress(int,int)) );
                  QObject::connect( databaseWorker, SIGNAL(finished()),
                                    qApp, SLOT(quit()) );
                  QTimer::singleShot( 0, databaseWorker, SLOT(start()) );
                  console.message( QObject::tr("updating %1").arg( baseDir ) );
                  app.exec();
               }
            }
         }
         else
         {
            stdErr << QObject::tr("Usage:\t%1 (%2) (%3 <directory>) (%4)\n")
                                  .arg( QApplication::applicationName(),
                                        _cleanup, _basedir, _update );
            return 1;
         }
      }
   }
   else
   {
      if( useGUI )
      {
         Q_INIT_RESOURCE( Common );
         SorcererLoader::detect();

         Satellite *s = GenericSatMsgHandler::createSatellite();
         {
            QFile qssFile( MySettings().styleSheetFile() );
            if( qssFile.exists() && qssFile.open( QIODevice::ReadOnly ) )
            {
               app.setStyleSheet( qssFile.readAll() );
               qssFile.close();
            }
         }

         MainWindow window;
         RubberbandmanMainWidget *mainWidget = new RubberbandmanMainWidget( &window );
         window.setMainWidget( mainWidget );
         window.show();

         if( s ) s->start();
         retval = app.exec();

         Satellite::destroy();
      }
   }

   DatabaseInterface::destroy();

   return retval;
}
