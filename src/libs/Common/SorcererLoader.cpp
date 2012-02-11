/*
 * src/libs/Common/SorcererLoader.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

/* class declaration */
#include "SorcererLoader.hpp"

/* system headers */
#include <cstdlib> // exit(), getenv()

/* Qt headers */
#include <QApplication>
#include <QDir>
#include <QMessageBox>
#include <QPluginLoader>
#include <QStringList>

/* local library headers */
#include "../../libs/Sorcerer/SorcererInterface.hpp"

/* local headers */
#include "Database.hpp"
#include "MySettings.hpp"
#include "Version.hpp"


QPluginLoader *SorcererLoader::cpPluginLoader = 0;


void SorcererLoader::detect( bool force )
{
   SorcererInterface *sorcerer = 0;
   MySettings settings;
   MySettings global( "Global" );
   QStringList regVersions( global.value("Version","0").toString().split('.') );
   QStringList thisVersions( QString( SLART_VERSION ).split('.') );
   unsigned long regVersion = 0;
   unsigned long thisVersion = 0;

   for( int i = 0; i < (int)sizeof(unsigned long); i++ )
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

   bool setup = force;
   bool cleanup = ( regVersion < thisVersion );
   bool hidden = ( ::getenv( "SLART_ADD_HIDDEN_REGISTRY_SETTINGS" ) != 0 );
   if( !settings.contains( "UseSatellite" ) || !Database::exists() )
   {
      setup = true;
   }
#if 0
   /* not needed right now, Satellite check is above */
   if( regVersion < 0x00075b00 )
   {
      /* Satellite came up with 0.7.91 */
      setup = true;
   }
#endif

   if( setup || cleanup || hidden )
   {
      sorcerer = tryLoading();
      if( !sorcerer )
      {
         QMessageBox::critical( 0, qApp->applicationName(),
                                QObject::tr("Loading Sorcerer failed!\nCannot start.\nSorry.") );
         ::exit( 1 );
      }

      if( setup )
      {
         if( sorcerer->setup() )
         {
            if( !force)
            {
               QMessageBox::critical( 0, qApp->applicationName(),
                                      QObject::tr("Setup failed!\nCannot start.\nSorry.") );
            }
            ::exit( 1 );
         }
      }
      if( cleanup )
      {
         sorcerer->cleanup();
      }
      if( hidden )
      {
         sorcerer->hidden();
      }
      unload();
   }
}


SorcererInterface *SorcererLoader::tryLoading()
{
   SorcererInterface *sorcerer = 0;
   QDir sorcererDir( qApp->applicationDirPath() );
#if defined Q_OS_MAC
   if( sorcererDir.dirName() == "MacOS" )
   {
      QDir searchDir( sorcererDir );
      searchDir.cdUp();
      if( searchDir.cd( "PlugIns" ) )
      {
         sorcerer = tryLoading( searchDir );
         if( sorcerer )
         {
            return sorcerer;
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
         sorcerer = tryLoading( searchDir );
         if( sorcerer )
         {
            return sorcerer;
         }
      }
   }
#endif
   return tryLoading( sorcererDir );
}


SorcererInterface *SorcererLoader::tryLoading( const QDir &dir )
{
   QString path( dir.absolutePath() );
#if defined Q_OS_WIN
   path.append( "/Sorcerer.dll" );
#elif defined Q_OS_MAC
   path.append( "/libSorcerer.dylib" );
#else
   path.append( "/libSorcerer.so" );
#endif
   if( !cpPluginLoader )
   {
      cpPluginLoader = new QPluginLoader();
   }
   cpPluginLoader->setFileName( path );
   QObject *plugin = cpPluginLoader->instance();
   if( plugin )
   {
      return qobject_cast<SorcererInterface *>(plugin);
   }
   return 0;
}


void SorcererLoader::unload()
{
   if( cpPluginLoader )
   {
      cpPluginLoader->unload();
      delete cpPluginLoader;
      cpPluginLoader = 0;
   }
}
