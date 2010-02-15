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
#include <QStringList>

#include <stdlib.h> // exit()

#include "../../apps/Sorcerer/Interface.hpp"
#include "MySettings.hpp"
#include "Database.hpp"
#include "Version.hpp"


void SorcererLoader::detect( QApplication *app, bool force )
{
   MySettings settings;
   MySettings global("Global");
   QStringList regVersions( global.value("Version","0").toString().split('.') );
   QStringList thisVersions( QString( SLART_VERSION ).split('.') );
   unsigned long regVersion = 0;
   unsigned long thisVersion = 0;
   for( int i = 0; i < (int)sizeof(long); i++ )
   {
      regVersion <<= 8;
      thisVersion <<= 8;
      if( i < regVersions.size() )
      {
         regVersion += regVersions.at(i).toULong();
      }
      if( i < thisVersions.size() )
      {
         thisVersion += thisVersions.at(i).toULong();
      }
   }

   if( !settings.contains( "UseSatellite" ) || !Database::exists() || force )
   {
      if( !run( app ) )
      {
         QMessageBox::critical( 0, app->applicationName(),
                                QObject::tr("Setup failed!\nCannot start.\nSorry.") );
         ::exit( 1 );
      }
   }

   if( regVersion < thisVersion )
   {
#if 0
      QMessageBox::information( 0, app->applicationName(),
                                QObject::tr("New version (%1 -> %2).\n"
                                            "Cleaning up registry.")
                                .arg(regVersions.join("."),thisVersions.join(".")) );
#endif
      cleanupSettings();
      global.setValue( "Version", SLART_VERSION );
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


void SorcererLoader::cleanupSettings()
{
   MySettings Global("Global");
   Global.remove("UseGlobalStyleSheetFile");

   MySettings Funkytown("Funkytown");
   cleanupSettings( &Funkytown );
   Funkytown.remove( "flv2mpeg4" );

   MySettings Innuendo("Innuendo");
   cleanupSettings( &Innuendo );
   Innuendo.remove( "Next" );
   Innuendo.remove( "Play" );
   Innuendo.remove( "Stop" );

   MySettings Karmadrome("Karmadrome");
   cleanupSettings( &Karmadrome );
   Karmadrome.remove("ButtonRows");

   MySettings Partyman("Partyman");
   cleanupSettings( &Partyman );
   Partyman.remove("DatabaseFilename");

   MySettings Rubberbandman("Rubberbandman");
   cleanupSettings( &Rubberbandman );

   MySettings Stripped("Stripped");
   cleanupSettings( &Stripped );
}


void SorcererLoader::cleanupSettings( MySettings *settings )
{
   if( settings )
   {
      settings->remove( "Listener" );
      settings->remove( "SLARTCommunication" );
      settings->remove( "StyleSheet" );
      settings->remove( "UDPListenerPort" );
   }
}
