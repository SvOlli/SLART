/**
 * src/apps/Sorcerer/Application.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#include <QApplication>

#include "SorcererLoader.hpp"


int main(int argc, char *argv[])
{
   int retval = 0;

   QApplication app(argc, argv);
   app.setOrganizationName("SLART");
   app.setOrganizationDomain("svolli.org");
   app.setApplicationName("Sorcerer");
   
   SorcererLoader::detect( &app, true );

   return retval;
}