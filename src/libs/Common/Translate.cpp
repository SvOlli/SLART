/*
 * src/libs/Common/Translate.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

/* class declaration */
#include "Translate.hpp"

/* system headers */

/* Qt headers */
#include <QCoreApplication>
#include <QDir>
#include <QLibraryInfo>
#include <QLocale>
#include <QTranslator>

/* local library headers */

/* local headers */

/* class variables */


Translate::Translate()
: mTranslators()
{
}


Translate::~Translate()
{
   foreach( QTranslator *translator, mTranslators )
   {
      delete translator;
   }
}


void Translate::install( QCoreApplication *app )
{
   if( !app )
   {
      app = qApp;
   }

   load( app, "qt" );
   load( app, "Common" );
   load( app, app->applicationName() );
}


void Translate::load( QCoreApplication *app, const QString &catalog )
{
   QTranslator *translator = new QTranslator();

   QString filename = QString( "%1_%2" ).arg( catalog, QLocale::system().name() );
   if( !translator->load( filename, location( app ) ) )
   {
      if( !translator->load( filename, QLibraryInfo::location( QLibraryInfo::TranslationsPath ) ) )
      {
         delete translator;
         translator = 0;
      }
   }
   if( translator )
   {
      app->installTranslator( translator );
   }
}


QString Translate::location( QCoreApplication *app )
{
   QDir dir( app->applicationDirPath() );
   /* try to change to translations directory */
#if defined Q_OS_MAC
   dir.cd( "../Resources/translations" );
#elif defined Q_OS_UNIX
   dir.cd( "../share/slart/translations" );
#endif
   /* if it failed, path of application is returned */
   return dir.absolutePath();
}
