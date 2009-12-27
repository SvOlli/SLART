/**
 * src/apps/Innuendo/Application.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#include "Database.hpp"
#include "MainWindow.hpp"
#include "MainWidget.hpp"
#include "MySettings.hpp"
#include "SorcererLoader.hpp"

#include <QtGui>


int main(int argc, char *argv[])
{
   int retval = 0;

   QApplication::setOrganizationName("SLART");
   QApplication::setOrganizationDomain("svolli.org");
   QApplication::setApplicationName("Innuendo");
   
   if( argc == 1 )
   {
      QApplication app(argc, argv);

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
      window.show();
      
      retval = app.exec();
   }
   else if( argc > 2 )
   {
      QCoreApplication app(argc, argv);
      
      QString application( argv[1] );
      QStringList message;
      for( int i = 2; i < argc; i++ )
      {
         message.append( argv[i] );
      }
      MySettings().sendUdpMessage( message.join("\n"), application );
   }
   else
   {
      fprintf( stderr, "Usage: %s (<application> <message>)\n", argv[0] );
      retval = 1;
   }
   
   return retval;
}
