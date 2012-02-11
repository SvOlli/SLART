/*
 * src/apps/Sorcerer/Application.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* system headers */

/* Qt headers */
#include <QApplication>

/* local library headers */
#include <SorcererLoader.hpp>
#include <Trace.hpp>
#include <Translate.hpp>

/* local headers */


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

   SorcererLoader::detect( true );

   return retval;
}
