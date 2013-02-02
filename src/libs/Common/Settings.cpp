/*
 * src/libs/Common/Settings.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

/* class declaration */
#include "Settings.hpp"

/* system headers */

/* Qt headers */
#include <QApplication>

/* local library headers */

/* local headers */

/* class variable instantiation */


Settings *Settings::cpSettings = 0;


Settings::Settings( QObject *parent )
: QObject( parent )
, mSettings()
{
}


Settings::~Settings()
{
   foreach( const QString &key, mSettings.keys() )
   {
      disconnect( mSettings.value( key ), SIGNAL(destroyed(QObject*)),
                  this, SLOT(settingsDestroyed(QObject*)) );
   }
}


QSettings *Settings::get( const QString &applicationName )
{
   QSettings *settings = 0;
   if( !cpSettings )
   {
      cpSettings = new Settings( QCoreApplication::instance() );
   }
   settings = cpSettings->mSettings.value( applicationName );
   if( !settings )
   {
      settings = new QSettings( QCoreApplication::organizationName(),
                                applicationName, cpSettings );
      cpSettings->mSettings.insert( applicationName, settings );
      connect( settings, SIGNAL(destroyed(QObject*)),
               cpSettings, SLOT(settingsDestroyed(QObject*)) );
   }

   return settings;
}


void Settings::settingsDestroyed( QObject *object )
{
   QSettings *settings = static_cast<QSettings*>(object);

   mSettings.remove( mSettings.key( settings ) );
}


bool Settings::remove( const QString &key, const QString &applicationName )
{
   bool removed = false;
   QStringList allApplications;
   if( applicationName.isEmpty() )
   {
      allApplications << "Funkytown"
                      << "Innuendo"
                      << "Karmadrome"
                      << "Notorious"
                      << "Partyman"
                      << "Rubberbandman"
                      << "Stripped";
   }
   else
   {
      allApplications << applicationName;
   }

   foreach( const QString &application, allApplications )
   {
      QSettings *settings = cpSettings->get( application );
      if( settings->contains( key ) )
      {
         removed = true;
         settings->remove( key );
      }
   }

   return removed;
}


bool Settings::exist()
{
   QSettings *settings = cpSettings->get();
   bool retval = settings->contains( "UseSatellite" );
   if( !retval )
   {
      delete settings; // prevent caching of settings changed by Sorcerer
   }
   return retval;
}


void Settings::setApplicationStyleSheet( QApplication *app )
{
   if( !app )
   {
      return;
   }

   QFile qssFile( Settings::value( Settings::CommonUseGlobalStyleSheetFile ) ?
                     Settings::value( Settings::GlobalStyleSheetFile ) :
                     Settings::value( Settings::CommonStyleSheetFile ) );
   if( qssFile.exists() && qssFile.open( QIODevice::ReadOnly ) )
   {
      app->setStyleSheet( qssFile.readAll() );
      qssFile.close();
   }
}
