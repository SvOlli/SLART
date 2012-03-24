/*
 * src/apps/Stripped/StrippedApplication.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* system headers */

/* Qt headers */
#include <QtGui>

/* local library headers */
#include <Satellite.hpp>
#include <Settings.hpp>
#include <SingleInstance.hpp>
#include <Trace.hpp>
#include <Translate.hpp>

/* local headers */
#include "StrippedConfigDialog.hpp"
#include "StrippedMainWindow.hpp"


int main(int argc, char *argv[])
{
   int retval = 0;

   QApplication::setOrganizationName("SLART");
   QApplication::setOrganizationDomain("svolli.org");
   QApplication::setApplicationName("Stripped");

   enableCore();

   QApplication app( argc, argv );

   Translate translate;
   translate.install( &app );

   Satellite::create();

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

   StrippedMainWindow window;
   window.show();

   retval = app.exec();

   Satellite::destroy();

   return retval;
}