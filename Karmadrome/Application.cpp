/**
 * Application.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#include "MainWindow.hpp"
#include "MainWidget.hpp"
#include "MySettings.hpp"
#include "Database.hpp"
#include "ImportExport.hpp"

#include <QtGui>

#include "Trace.hpp"


int main(int argc, char *argv[])
{
   int retval = 0;

   QApplication::setOrganizationName("SLART");
   QApplication::setOrganizationDomain("svolli.org");
   QApplication::setApplicationName("Karmadrome");
   
   if( (argc > 1) && strcmp(argv[1],"-qws") )
   {
      QCoreApplication app(argc, argv);
      bool fail = false;
      
      bool clean       = false;
      bool shuffle     = false;
      bool relative    = false;
      bool doimport    = false;
      bool doexport    = false;
      bool dolist      = false;
      char *foldername = (char*)0;
      char *filename   = (char*)0;
      int opt;
      
      if( !Database::exists() )
      {
         return 2;
      }
      
      while ((opt = getopt(argc, argv, "hcsrlien:f:")) != -1)
      {
         switch(opt)
         {
            case 'c':
               clean = true;
               break;
            case 's':
               shuffle = true;
               break;
            case 'r':
               relative = true;
               break;
            case 'l':
               dolist = true;
               break;
            case 'i':
               doimport = true;
               break;
            case 'e':
               doexport = true;
               break;
            case 'n':
               foldername = optarg;
               break;
            case 'f':
               filename = optarg;
               break;
            case 'h':
            default:
               fail = true;
         }
      }
      
      if( dolist )
      {
         if( foldername || doimport || doexport ||
             clean || relative || shuffle )
         {
            fail = true;
         }
      }
      else
      {
         if( !foldername )
         {
            fail = true;
         }
         if( !filename )
         {
            fail = true;
         }
         if( doimport == doexport )
         {
            fail = true;
         }
         if( doimport && (relative || shuffle) )
         {
            fail = true;
         }
         if( doexport && clean )
         {
            fail = true;
         }
      }
      
      if( fail )
      {
         fprintf( stderr, "Usage:\t%s\n"
                  "list:\t%s -l [-f filename ]\n"
                  "import:\t%s -i -n foldername -f list.m3u [-c]\n"
                  "export:\t%s -e -n foldername -f list.m3u [-r] [-s]\n",
                  argv[0], argv[0], argv[0], argv[0] );
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
         ie.importM3u( QString(foldername), QString(filename), clean );
      }
      else if( doexport )
      {
         ie.exportM3u( QString(foldername), QString(filename), relative, shuffle );
      }
   }
   else
   {
      QApplication app(argc, argv);

      if( !MySettings().contains( "SLARTCommunication" ) || !Database::exists() )
      {
         if( !MainWindow::invokeSetUp( &app ) )
         {
            return 2;
         }
      }
      
      MainWindow window;
      window.show();
      
      retval = app.exec();
   }
   
   return retval;
}
