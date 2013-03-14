/*
 * src/apps/Funkytown/FunkytownApplication.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* system headers */

/* Qt headers */
#include <QApplication>
#include <QFileInfo>

/* local library headers */
#include <GenericSatelliteHandler.hpp>
#include <MainWindow.hpp>
#include <Settings.hpp>
#include <Satellite.hpp>
#include <SorcererLoader.hpp>
#include <Trace.hpp>
#include <Translate.hpp>

/* local headers */
#include "FunkytownMainWidget.hpp"


int main(int argc, char *argv[])
{
   int retval = 0;

   QApplication::setOrganizationName("SLART");
   QApplication::setOrganizationDomain("svolli.org");
   QApplication::setApplicationName("Funkytown");

   enableCore();

   QApplication app( argc, argv );
   Settings::setApplicationStyleSheet( &app );

   Translate *translate = new Translate( &app );
   translate->install();

   QStringList *startUrls = new QStringList();

   SorcererLoader::detect();

   GenericSatelliteHandler::createSatellite();

   for( retval = 1; retval < argc; retval++ )
   {
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
            line = QString::fromLocal8Bit( input.readLine().constData() );
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
   FunkytownMainWidget *mainWidget = new FunkytownMainWidget( &window );
   window.setMainWidget( mainWidget );
   foreach( const QString &url, *startUrls )
   {
      mainWidget->downloadUserPage( url );
   }
   delete startUrls;
   window.show();
   retval = app.exec();

   Satellite::destroy();

   return retval;
}
