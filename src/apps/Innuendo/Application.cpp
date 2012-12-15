/*
 * src/apps/Innuendo/Application.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* system headers */

/* Qt headers */
#include <QtGui>

/* local library headers */
#include <GenericSatMsgHandler.hpp>
#include <MainWindow.hpp>
#include <MySettings.hpp>
#include <Satellite.hpp>
#include <SorcererLoader.hpp>
#include <Trace.hpp>
#include <Translate.hpp>

/* local headers */
#include "InnuendoMainWidget.hpp"


int main(int argc, char *argv[])
{
   int retval = 0;

   QApplication::setOrganizationName("SLART");
   QApplication::setOrganizationDomain("svolli.org");
   QApplication::setApplicationName("Innuendo");

   enableCore();

   if( argc == 1 )
   {
      QApplication app( argc, argv );

      Translate translate;
      translate.install( &app );

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
      InnuendoMainWidget *mainWidget = new InnuendoMainWidget( &window );
      window.setMainWidget( mainWidget );
      window.show();

      if( s ) s->start();
      retval = app.exec();

      Satellite::destroy();
   }
   else
   {
      QCoreApplication app( argc, argv );

      QStringList message;
      for( int i = 1; i < argc; i++ )
      {
         message.append( argv[i] );
      }
      GenericSatMsgHandler::send( message.join("\n").toUtf8() );
   }

   return retval;
}
