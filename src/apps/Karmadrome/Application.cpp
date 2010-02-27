/**
 * src/apps/Karmadrome/Application.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* system headers */
#include <stdlib.h>

/* Qt headers */
#include <QtGui>

/* local library headers */
#include <Database.hpp>
#include <MainWindow.hpp>
#include <MySettings.hpp>
#include <SorcererLoader.hpp>

/* local headers */
#include "ImportExport.hpp"
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
   QApplication app(argc, argv, useGUI);
   
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
      
      if( !Database::exists() )
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
         fprintf( stderr, "Usage:\t%s\n"
                  "\t%s %s (filename)\n"
                  "\t%s %s <foldername> <filename.m3u> (%s)\n"
                  "\t%s %s <foldername> <filename.m3u> (%s) (%s)\n",
                  argv[0], 
                  argv[0], _list.toLocal8Bit().constData(),
                  argv[0], _import.toLocal8Bit().constData(), _clean.toLocal8Bit().constData(),
                  argv[0], _export.toLocal8Bit().constData(),
                           _relative.toLocal8Bit().constData(), _shuffle.toLocal8Bit().constData() );
         return 1;
      }
      
      Database     db;
      ImportExport ie( &db );
      
      if( dolist )
      {
         ie.listFolders( QString( filename ) );
      }
      else if( doimport )
      {
         ie.importM3u( foldername, filename, withclean );
      }
      else if( doexport )
      {
         ie.exportM3u( foldername, filename, withrelative, withshuffle );
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
