/*
 * src/libs/Common/GSettingsWrapper.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

/* class declaration */
#include "GSettingsWrapper.hpp"

/* system headers */
#ifdef signals
// implement a workaround, since "signals" is used in glib headers
#undef signals
#include <gio/gio.h>
#define signals Q_SIGNALS
#else
#include <gio/gio.h>
#endif
#include <cstring>
#include <cstdlib>

/* Qt headers */

/* local library headers */

/* local headers */
#include "Settings.hpp"

/* class variables */


GSettingsWrapper::GSettingsWrapper( const QString &domain )
: mpGSettings( 0 )
{
#if GLIB_VERSION_2_26
   g_type_init();
   QByteArray domainUtf8( domain.toUtf8() );
#if 1
   const gchar*const *schemasList = g_settings_list_schemas();
#else
   const gchar*const *schemasList;
   g_settings_schema_source_list_schemas();
#endif
   for( const gchar*const *entry = schemasList; *entry; ++entry )
   {
      if( !::strcasecmp( domainUtf8.constData(), *entry ) )
      {
         GSettings *gsettings = g_settings_new( domain.toUtf8().constData() );
         mpGSettings = static_cast<void*>( gsettings );
         break;
      }
   }
#endif
}


GSettingsWrapper::~GSettingsWrapper()
{
#if GLIB_VERSION_2_26
   GSettings *gsettings = static_cast<GSettings*>( mpGSettings );
   Q_UNUSED( gsettings );
   // would like to run g_free here, but it crashes applications
   //g_free( gsettings );
#endif
}


bool GSettingsWrapper::isAvailable()
{
#if GLIB_VERSION_2_26
   return true;
#else
   return false;
#endif
}


bool GSettingsWrapper::isValid()
{
   return mpGSettings ? true : false;
}


bool GSettingsWrapper::valueBool( const QString &key )
{
   bool retval = false;
#if GLIB_VERSION_2_26
   GSettings *gsettings = static_cast<GSettings*>( mpGSettings );
   if( gsettings )
   {
      retval = g_settings_get_boolean( gsettings, key.toUtf8().constData() );
   }
#endif
   return retval;
}


bool GSettingsWrapper::setValueBool( const QString &key, bool value )
{
   bool ok = false;
#if GLIB_VERSION_2_26
   GSettings *gsettings = static_cast<GSettings*>( mpGSettings );
   if( gsettings )
   {
      ok = g_settings_set_boolean( gsettings, key.toUtf8().constData(), value );
   }
#endif
   return ok;
}


int GSettingsWrapper::valueInt( const QString &key )
{
   int retval = 0;
#if GLIB_VERSION_2_26
   GSettings *gsettings = static_cast<GSettings*>( mpGSettings );
   if( gsettings )
   {
      retval = g_settings_get_int( gsettings, key.toUtf8().constData() );
   }
#endif
   return retval;
}


bool GSettingsWrapper::setValueInt( const QString &key, int value )
{
   bool ok = false;
#if GLIB_VERSION_2_26
   GSettings *gsettings = static_cast<GSettings*>( mpGSettings );
   if( gsettings )
   {
      ok = g_settings_set_int( gsettings, key.toUtf8().constData(), value );
   }
#endif
   return ok;
}


QString GSettingsWrapper::valueString( const QString &key )
{
   QString retval;
#if GLIB_VERSION_2_26
   GSettings *gsettings = static_cast<GSettings*>( mpGSettings );
   if( gsettings )
   {
      gchar *value = g_settings_get_string( gsettings, key.toUtf8().constData() );
      if( value )
      {
         retval = static_cast<char*>( value );
         g_free( value );
      }
   }
#endif
   return retval;
}


bool GSettingsWrapper::setValueString( const QString &key, const QString &value )
{
   bool ok = false;
#if GLIB_VERSION_2_26
   GSettings *gsettings = static_cast<GSettings*>( mpGSettings );
   if( gsettings )
   {
      ok = g_settings_set_string( gsettings, key.toUtf8().constData(), value.toUtf8().constData() );
   }
#endif
   return ok;
}


QStringList GSettingsWrapper::valueStringList( const QString &key )
{
   QStringList retval;
#if GLIB_VERSION_2_26
   GSettings *gsettings = static_cast<GSettings*>( mpGSettings );
   if( gsettings )
   {
      gchar **list = g_settings_get_strv( gsettings, key.toUtf8().constData() ) ;
      for( gchar **entry = list; *entry; ++entry )
      {
         retval << *entry;
         g_free( *entry );
      }
      g_free( list );
   }
#endif
   return retval;
}


bool GSettingsWrapper::setValueStringList( const QString &key, const QStringList &value )
{
   bool ok = false;
#if GLIB_VERSION_2_26
   GSettings *gsettings = static_cast<GSettings*>( mpGSettings );
   if( gsettings )
   {
      QList<QByteArray> balist;
      const gchar **list = new const gchar*[value.size() + 1];
      list[value.size()] = 0;
      for( int i = 0; i < value.size(); ++i )
      {
         balist.append( value.at(i).toUtf8() );
         list[i] = balist[i].constData();
      }
      ok = g_settings_set_strv( gsettings, key.toUtf8().constData(), list );
      delete[] list;
   }
#endif
   return ok;
}
