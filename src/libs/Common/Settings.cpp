/*
 * src/libs/Common/Settings.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "Settings.hpp"

/* system headers */

/* Qt headers */
//#include <QApplication>

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
                  this, SLOT(mySettingsDestroyed(QObject*)) );
   }
}


MySettings *Settings::get( const QString &applicationName )
{
   MySettings *settings = 0;
   if( !cpSettings )
   {
      cpSettings = new Settings( QCoreApplication::instance() );
   }
   settings = cpSettings->mSettings.value( applicationName );
   if( !settings )
   {
      settings = new MySettings( applicationName, cpSettings );
      cpSettings->mSettings.insert( applicationName, settings );
      connect( settings, SIGNAL(destroyed(QObject*)),
               cpSettings, SLOT(mySettingsDestroyed(QObject*)) );
   }

   return settings;
}


void Settings::mySettingsDestroyed( QObject *object )
{
   MySettings *settings = static_cast<MySettings*>(object);

   mSettings.remove( mSettings.key( settings ) );
}


void Settings::remove( const QString &key, const QString &applicationName )
{
   QStringList allApplications;
   allApplications << "Funkytown"
                   << "Innuendo"
                   << "Karmadrome"
                   << "Notorious"
                   << "Partyman"
                   << "Rubberbandman"
                   << "Stripped";

   if( applicationName.isEmpty() )
   {
      foreach( const QString &application, allApplications )
      {
         cpSettings->get( application )->remove( key );
      }
   }
   else
   {
      cpSettings->get( applicationName )->remove( key );
   }
}
