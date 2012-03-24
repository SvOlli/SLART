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
