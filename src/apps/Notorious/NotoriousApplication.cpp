/*
 * src/apps/Notorious/NotoriousApplication.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* system headers */

/* Qt headers */
#include <QApplication>

/* local library headers */
#include <MainWindow.hpp>
#include <Settings.hpp>
#include <SorcererLoader.hpp>
#include <Trace.hpp>
#include <Translate.hpp>

/* local headers */
#include "NotoriousMainWidget.hpp"


int main(int argc, char *argv[])
{
   int retval = 0;

   QApplication::setOrganizationName("SLART");
   QApplication::setOrganizationDomain("svolli.org");
   QApplication::setApplicationName("Notorious");

   enableCore();

   QApplication app( argc, argv );
   Settings::setApplicationStyleSheet( &app );

   Translate *translate = new Translate( &app );
   translate->install();

   MainWindow window;
   NotoriousMainWidget *mainWidget = new NotoriousMainWidget( &window );
   window.setMainWidget( mainWidget );
   window.show();

   retval = app.exec();

   return retval;
}
