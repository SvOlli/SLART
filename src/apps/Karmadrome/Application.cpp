/**
 * src/apps/Karmadrome/Application.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* system headers */
#include <cstdlib>

/* Qt headers */
#include <QtGui>

/* local library headers */
#include <DatabaseInterface.hpp>
#include <MainWindow.hpp>
#include <MySettings.hpp>
#include <SorcererLoader.hpp>
#include <Translate.hpp>

/* local headers */
#include "ExportFolder.hpp"
#include "ExportFolderList.hpp"
#include "ImportFolder.hpp"
#include "MainWidget.hpp"


int main(int argc, char *argv[])
{
   int retval = 0;

   QApplication::setOrganizationName("SLART");
   QApplication::setOrganizationDomain("svolli.org");
   QApplication::setApplicationName("Karmadrome");

#ifdef Q_WS_X11
   bool useGUI = getenv("DISPLAY") != 0;
#else
   bool useGUI = true;
#endif
   QApplication app( argc, argv, useGUI );

   Translate translate;
   translate.install( &app );

   QStringList args( QApplication::arguments() );
   if( args.size() > 1 )
   {
      args.takeFirst(); // first argument is program name
      bool fail = false;

      bool withclean    = false;
      bool withshuffle  = false;
      bool withrelative = false;
      bool doimport   = false;
      bool doexport   = false;
      bool dolist     = false;

      const QString _list    ( "-list" );
      const QString _import  ( "-import" );
      const QString _export  ( "-export" );
      const QString _clean   ( "-clean" );
      const QString _relative( "-relative" );
      const QString _shuffle ( "-shuffle" );

      QString foldername;
      QString filename;
      QString arg;

      if( !DatabaseInterface::exists() )
      {
         return 2;
      }

      arg = args.takeFirst();
      if( arg == _list )
      {
         dolist = true;
         switch( args.size() )
         {
            case 0:
               break;
            case 1:
               filename = args.takeFirst();
               break;
            default:
               fail = true;
               break;
         }
      }
      else if( arg == _import )
      {
         doimport = true;
         if( args.size() < 2 )
         {
            fail = true;
         }
         else
         {
            foldername = args.takeFirst();
            filename   = args.takeFirst();
         }
         while( args.size() > 0 )
         {
            arg = args.takeFirst();
            if( arg == _clean )
            {
               withclean = true;
            }
            else
            {
               fail = true;
            }
         }
      }
      else if( arg == _export )
      {
         doexport = true;
         if( args.size() < 1 )
         {
            fail = true;
         }
         else
         {
            foldername = args.takeFirst();
            if( args.size() > 0 )
            {
               filename   = args.takeFirst();
            }
         }
         while( args.size() > 0 )
         {
            arg = args.takeFirst();
            if( arg == _relative )
            {
               withrelative = true;
            }
            else if( arg == _shuffle )
            {
               withshuffle = true;
            }
            else
            {
               fail = true;
            }
         }
      }
      else
      {
         fail = true;
      }

      if( fail )
      {
         QTextStream stdErr( ::stderr, QIODevice::WriteOnly );
         stdErr << QString("Usage:\t%1\n"
                  "\t%2 %3 (filename)\n"
                  "\t%4 %5 <foldername> <filename.m3u> (%6)\n"
                  "\t%7 %8 <foldername> <filename.m3u> (%9) (%10)\n")
                  .arg( QApplication::applicationName() )
                  .arg( QApplication::applicationName(), _list )
                  .arg( QApplication::applicationName(), _import, _clean )
                  .arg( QApplication::applicationName(), _export, _relative, _shuffle );
         return 1;
      }

      QObject *object = 0;
      if( dolist )
      {
         ExportFolderList *exportFolderList =
               new ExportFolderList( filename );
         object = qobject_cast<QObject*>(exportFolderList);
      }
      else if( doimport )
      {
         ImportFolder *importFolder =
               new ImportFolder( foldername, filename, withclean );
         object = qobject_cast<QObject*>(importFolder);
      }
      else if( doexport )
      {
         ExportFolder *exportFolder =
               new ExportFolder( foldername, filename, withrelative, withshuffle );
         object = qobject_cast<QObject*>(exportFolder);
      }
      QObject::connect( object, SIGNAL(destroyed()),
                        &app, SLOT(quit()) );
      retval = app.exec();
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

   DatabaseInterface::destroy();
   return retval;
}
