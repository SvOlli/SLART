/**
 * src/libs/CommandLine/SorcererLoader.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#include "SorcererLoader.hpp"

#include <QApplication>
#include <QDir>
#include <QMessageBox>
#include <QPluginLoader>

#include <stdlib.h> // exit()

#include "../../apps/Sorcerer/Interface.hpp"
#include "MySettings.hpp"
#include "Database.hpp"


void SorcererLoader::detect( QApplication *app, bool force )
{
   if( !MySettings("Global").contains( "UseSatellite" ) || !Database::exists() || force )
   {
      if( !run( app ) )
      {
         QMessageBox::critical( 0, app->applicationName(),
                                QObject::tr("Setup failed!\nCannot start.\nSorry.") );
         ::exit( 1 );
      }
   }
}


bool SorcererLoader::run( QApplication *app )
{
   if( !app )
   {
      app = qApp;
   }
   QDir sorcererDir(app->applicationDirPath());
#if defined Q_OS_MAC
   if( sorcererDir.dirName() == "MacOS" )
   {
      QDir searchDir( sorcererDir );
      searchDir.cdUp();
      if( searchDir.cd( "PlugIns" ) )
      {
         if( tryLoading( app, searchDir ) )
         {
            return true;
         }
      }
   }
#elif defined Q_OS_UNIX
   if( sorcererDir.dirName() == "bin" )
   {
      QDir searchDir( sorcererDir );
      searchDir.cdUp();
      if( searchDir.cd( "lib" ) )
      {
         if( tryLoading( app, searchDir ) )
         {
            return true;
         }
      }
   }
#endif
   if( tryLoading( app, sorcererDir ) )
   {
      return true;
   }

#if 0
   QMessageBox::critical( 0, app->applicationName(),
                          QObject::tr("Setup failed!\nCannot start.\nSorry.") );
#endif

   return false;
}


bool SorcererLoader::tryLoading( QApplication *app, const QDir &dir )
{
   int retval = 1;
   QString path( dir.absolutePath() );
#if defined Q_OS_WIN
   path.append( "/Sorcerer.dll" );
#elif defined Q_OS_MAC
   path.append( "/libSorcerer.dylib" );
#else
   path.append( "/libSorcerer.so" );
#endif
   QPluginLoader pluginLoader( path );
   QObject *plugin = pluginLoader.instance();
   if( plugin )
   {
      SorcererInterface *sorcerer = qobject_cast<SorcererInterface *>(plugin);
      if( sorcerer )
      {
         retval = sorcerer->run( app );
      }
   }

   return retval == 0;
}
