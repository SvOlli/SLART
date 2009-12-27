/**
 * src/apps/Funkytown/Application.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#include <unistd.h>

#include <QApplication>
#include <QFileInfo>

#include "MainWindow.hpp"
#include "MainWidget.hpp"
#include "MySettings.hpp"

#include "SorcererLoader.hpp"

//#include "Trace.hpp"


int main(int argc, char *argv[])
{
   int retval = 0;
   
   QApplication app(argc, argv);
   app.setOrganizationName("SLART");
   app.setOrganizationDomain("svolli.org");
   app.setApplicationName("Funkytown");
   QStringList *startUrls = new QStringList();
   MySettings settings;
   
   SorcererLoader::detect( &app );
   {
      QFile qssFile( settings.styleSheetFile() );
      if( qssFile.exists() && qssFile.open( QIODevice::ReadOnly ) )
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
   MainWidget *mainWidget = new MainWidget( &window );
   window.setMainWidget( mainWidget );
   for( retval = 0; retval < startUrls->size(); retval++ )
   {
      mainWidget->downloadUserPage( startUrls->at(retval) );
   }
   delete startUrls;
   window.show();
   retval = app.exec();
   
   return retval;
}
