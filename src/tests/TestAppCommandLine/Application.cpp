/**
 * src/tests/TestAppCommandLine/Application.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

/* system headers */

/* Qt headers */
#include <QCoreApplication>
#include <QtDebug>
#include <QStringList>

/* local library headers */
#include <CommandLine.hpp>

/* local headers */


int main( int argc, char *argv[] )
{
   QCoreApplication app( argc, argv );
   CommandLine cl;
   QString stringtest;
   QString stringdefaulttest("defaultstring");
   QStringList otherArgs;
   bool booltest = false;
   bool help = false;

   cl.option( "-help",        "show help",                &help,      true );
   cl.option( "-enablebool",  "boolean test set",         &booltest,  true );
   cl.option( "-disablebool", "boolean test unset",       &booltest,  false );
   cl.option( "-string",      "string test",              &stringtest );
   cl.option( "-default",     "string with default test", &stringdefaulttest );

   cl.parse( &otherArgs );

   if( cl.check() )
   {
      qDebug() << "fail! try '-help'";
      return 0;
   }

   if( help )
   {
      qDebug() << cl.help().toLocal8Bit().constData();
      return 0;
   }

   qDebug() << "booltest:"         << booltest;
   qDebug() << "stringtest:"       << stringtest;
   qDebug() << "stringdefaultest:" << stringdefaulttest;
   qDebug() << "other arguments:"  << otherArgs;

   return 0;
}
