/**
 * Application.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#include <unistd.h>

#include <QApplication>
#include <QFileInfo>

#include "MainWindow.hpp"
#include "MainWidget.hpp"

#include "Trace.hpp"

int main(int argc, char *argv[])
{
   int retval = 0;
   
   QApplication app(argc, argv);
   app.setOrganizationName("SLART");
   app.setOrganizationDomain("svolli.org");
   app.setApplicationName("Funkytown");
   QStringList *startUrls = new QStringList();
   
   for( retval = 1; retval < argc; retval++ )
   {
      QFileInfo qfi( argv[retval] );
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
            line = QString::fromLocal8Bit( input.readLine() );
            if( line.startsWith("http") )
            {
               if( line.right(1) == QChar('\n') )
               {
                  line.chop(1);
               }
               *startUrls << line;
            }
         }
         input.close();
      }
      else
      {
         *startUrls << QString( argv[retval] );
      }
   }

   MainWindow window;
   for( retval = 0; retval < startUrls->size(); retval++ )
   {
      window.mainWidget()->downloadUserPage( startUrls->at(retval) );
   }
   delete startUrls;
   window.show();
   retval = app.exec();
   
   return retval;
}
