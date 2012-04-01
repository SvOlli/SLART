/*
 * src/apps/Sorcerer/Application.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* system headers */

/* Qt headers */
#include <QApplication>

/* local library headers */
#include <DatabaseInterface.hpp>
#include <MainWindow.hpp>
#include <Satellite.hpp>
#include <Settings.hpp>
#include <Trace.hpp>
#include <Translate.hpp>

/* local headers */
#include "SorcererMainWidget.hpp"


int main(int argc, char *argv[])
{
   int retval = 0;

   QApplication::setOrganizationName("SLART");
   QApplication::setOrganizationDomain("svolli.org");
   QApplication::setApplicationName("Sorcerer");

   enableCore();

   QApplication app( argc, argv );

   Translate translate;
   translate.install( &app );

   Satellite::create();
   DatabaseInterface::create();

   {
      QFile qssFile( Settings::value( Settings::CommonUseGlobalStyleSheetFile ) ?
                        Settings::value( Settings::GlobalStyleSheetFile ) :
                        Settings::value( Settings::CommonStyleSheetFile ) );
      if( qssFile.exists() && qssFile.open( QIODevice::ReadOnly ) )
      {
         app.setStyleSheet( qssFile.readAll() );
         qssFile.close();
      }
   }

   MainWindow window;
   SorcererMainWidget *mainWidget = new SorcererMainWidget( &window );
   window.setMainWidget( mainWidget );
   window.show();

   retval = app.exec();

   DatabaseInterface::destroy();
   Satellite::destroy();

   return retval | mainWidget->errors();
}
