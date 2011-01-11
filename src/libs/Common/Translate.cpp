/**
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

/* local library headers */

/* local headers */

/* class variables */


#include <Trace.hpp>

Translate::Translate()
: mpQtTranslator( new QTranslator() )
, mpLibTranslator( new QTranslator() )
, mpAppTranslator( new QTranslator() )
{
}


Translate::~Translate()
{
   delete mpQtTranslator;
   delete mpLibTranslator;
   delete mpAppTranslator;
}


void Translate::install( QCoreApplication *app )
{
   if( !app )
   {
      app = qApp;
   }
   /* adopted from the Qt help */
   mpQtTranslator->load( "qt_" + QLocale::system().name(),
                         QLibraryInfo::location( QLibraryInfo::TranslationsPath ) );
   app->installTranslator( mpQtTranslator );

   mpLibTranslator->load( "Common_" + QLocale::system().name(),
                          location( app ) );
   app->installTranslator( mpLibTranslator );

   mpAppTranslator->load( app->applicationName() + "_" + QLocale::system().name(),
                          location( app ) );
   app->installTranslator( mpAppTranslator );
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
