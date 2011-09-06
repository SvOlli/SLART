/**
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
#include <QDir>

/* local library headers */

/* local headers */

/* class variable instantiation */
QPointer<MySettings> Settings::cpSettings = 0;


void Settings::createGlobal( QObject *parent )
{
   if( !cpSettings )
   {
      cpSettings = new MySettings( parent );
   }
}


void Settings::destroyGlobal()
{
   if( cpSettings )
   {
      delete cpSettings;
      cpSettings = 0;
   }
}


bool Settings::value( enum StrippedBool id )
{
   bool retval = false;
   cpSettings->beginGroup( "Stripped" );
   switch( id )
   {
   case StrippedAutoEject:
      retval = cpSettings->value( "AutoEject", false ).toBool();
   case StrippedAutoEnqueue:
      retval = cpSettings->value( "AutoEnqueue", false ).toBool();
   case StrippedAutoFreeDB:
      retval = cpSettings->value( "AutoFreeDB", true ).toBool();
   case StrippedCDTextLatin1:
      retval = cpSettings->value( "CDTextLatin1", false ).toBool();
   case StrippedDirectoryOverride:
      retval = cpSettings->value( "DirectoryOverride", false ).toBool();
   case StrippedShowStats:
      retval = cpSettings->value( "ShowStats", false ).toBool();
   default:
      qFatal( "illegal ParameterBool value" );
   }
   cpSettings->endGroup();
   return retval;
}


void Settings::setValue( enum StrippedBool id, bool value )
{
   cpSettings->beginGroup( "Stripped" );
   switch( id )
   {
   case StrippedAutoEject:
      cpSettings->setValue( "AutoEject", value );
   case StrippedAutoEnqueue:
      cpSettings->setValue( "AutoEnqueue", value );
   case StrippedAutoFreeDB:
      cpSettings->setValue( "AutoFreeDB", value );
   case StrippedCDTextLatin1:
      cpSettings->setValue( "CDTextLatin1", value );
   case StrippedDirectoryOverride:
      cpSettings->setValue( "DirectoryOverride", value );
   case StrippedShowStats:
      cpSettings->setValue( "ShowStats", value );
   default:
      qFatal( "illegal ParameterBool value" );
   }
   cpSettings->endGroup();
}


QString Settings::value( enum StrippedString id )
{
   QString retval;
   cpSettings->beginGroup( "Stripped" );
   switch( id )
   {
   case StrippedCreatePattern:
      retval = cpSettings->value( "CreatePattern", "|$ALBUMARTIST|/|$ALBUM|/(|#2TRACKNUMBER|)|$ARTIST| - |$TITLE|" ).toString();
   case StrippedDevice:
      retval = cpSettings->value( "Device", QString("/dev/cdrom") ).toString();
   case StrippedDirectory:
      retval = cpSettings->value( "Directory", QDir::current().absolutePath() ).toString();
   case ParameterStyleSheet:
      retval = cpSettings->value( "StyleSheet", QString() ).toString();
   default:
      qFatal( "illegal ParameterString value" );
   }
   cpSettings->endGroup();
   return retval;
}


void Settings::setValue( enum StrippedString id, const QString &value )
{
   cpSettings->beginGroup( "Stripped" );
   switch( id )
   {
   case StrippedCreatePattern:
      cpSettings->setValue( "CreatePattern", value );
   case StrippedDevice:
      cpSettings->setValue( "Device", value );
   case StrippedDirectory:
      cpSettings->setValue( "Directory", value );
   case ParameterStyleSheet:
      cpSettings->setValue( "StyleSheet", value );
   default:
      qFatal( "illegal ParameterString value" );
   }
   cpSettings->endGroup();
}
