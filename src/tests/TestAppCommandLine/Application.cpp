/**
 * main.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#include <CommandLine.hpp>

#include <QCoreApplication>
#include <QtDebug>
#include <QStringList>


int main( int argc, char *argv[] )
{
   QCoreApplication app( argc, argv );
   CommandLine cl;
   QString stringtest;
   QStringList otherArgs;
   bool booltest = false;
   bool help = false;
   
   cl.option( "-help",        "show help",          &help,      true );
   cl.option( "-enablebool",  "boolean test set",   &booltest,  true );
   cl.option( "-disablebool", "boolean test unset", &booltest,  false );
   cl.option( "-string",      "string test",        &stringtest );
   
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
   
   qDebug() << "booltest:"        << booltest;
   qDebug() << "stringtest:"      << stringtest;
   qDebug() << "other arguments:" << otherArgs;
   
   return 0;
}
