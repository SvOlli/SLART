/*
 * src/tools/SettingsGenerator/SettingsGeneratorStorage.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

/* class declaration */
#include "SettingsGeneratorStorage.hpp"

/* system headers */

/* Qt headers */
#include <QtDebug>
#include <QCoreApplication>
#include <QStringList>

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */

/* forward declaration of local classes */


SettingsGeneratorStorage::SettingsGeneratorStorage( const QString &applicationName,
                                                    const QString &type )
: mApplicationName( applicationName )
, mParts( mApplicationName.split(":") )
, mTopPart( mParts.takeFirst() )
, mType( type )
, mTypeName( mType.at(0).isUpper() ? "const " + mType + " &" : mType + " " )
, mParameterNames()
, mDefaultValues()
, mDeleteNames()
{
}


SettingsGeneratorStorage::~SettingsGeneratorStorage()
{
}


SettingsGeneratorStorage::SettingsGeneratorStorage( const SettingsGeneratorStorage &that )
: mApplicationName( that.mApplicationName )
, mParts( that.mParts )
, mTopPart( that.mTopPart )
, mType( that.mType )
, mTypeName( that.mTypeName )
, mParameterNames( that.mParameterNames )
, mDefaultValues( that.mDefaultValues )
, mDeleteNames( that.mDeleteNames )
{
}


SettingsGeneratorStorage &SettingsGeneratorStorage::operator=( const SettingsGeneratorStorage &that )
{
   mApplicationName  = that.mApplicationName;
   mParts            = that.mParts;
   mTopPart          = that.mTopPart;
   mType             = that.mType;
   mTypeName         = that.mTypeName;
   mParameterNames   = that.mParameterNames;
   mDefaultValues    = that.mDefaultValues;
   mDeleteNames      = that.mDeleteNames;

   return *this;
}


QString SettingsGeneratorStorage::enumTypeName( int i ) const
{
   QString typeName( mType );
   if( i < 0 )
   {
      if( typeName.startsWith( "Q", Qt::CaseInsensitive ) )
      {
         typeName = typeName.mid( 1 );
      }
      typeName = mApplicationName + typeName.left(1).toUpper() + typeName.mid(1);
   }
   else
   {
      typeName = mApplicationName + mParameterNames.at(i);
   }
   typeName.remove( QRegExp("[@:]") );

   return typeName;
}


void SettingsGeneratorStorage::addParameter( const QString &name, const QString &defaultValue )
{
   if( (mType == "-") || (defaultValue == "@delete") )
   {
      mDeleteNames    << name;
   }
   else
   {
      mParameterNames << name;
      mDefaultValues  << defaultValue;
   }
}


QStringList SettingsGeneratorStorage::enums() const
{
   QStringList retval;

   if( mType == "-" )
   {
      return retval;
   }

   retval << "/*!"
          << " \\brief enum for value() and setValue()\n"
          << " automatically generated"
          << " */"
          << "enum " + enumTypeName()
          << "{";

   QString enumName;
   Q_ASSERT( mParameterNames.size() == mDefaultValues.size() );
   for( int i = 0; i < mParameterNames.size(); i++ )
   {
      enumName = mApplicationName + mParameterNames.at(i);
      enumName.remove( QRegExp("[@:]") );
      retval << "\t" + enumName + ( (i + 1) < mParameterNames.size() ? "," : "" );
   }
   retval << "};" ;

   return retval;
}


QStringList SettingsGeneratorStorage::declaration() const
{
   QStringList retval;

   if( mType == "-" )
   {
      return retval;
   }

   retval << "/*!"
          << " \\brief set a value in registry\n"
          << " automatically generated"
          << " \\param id id of registry key"
          << " \\param value to set"
          << " */"
          << "static void setValue( " + enumTypeName() + " id, " + mTypeName + "value );\n"
          << "/*!"
          << " \\brief get a value in registry\n"
          << " automatically generated"
          << " \\return value in registry"
          << " \\param id id of registry key"
          << " */"
          << "static " + mType + " value( " + enumTypeName() + " id );\n"
          ;

   return retval;
}


QStringList SettingsGeneratorStorage::definition() const
{
   QString entryFormat("");
   QStringList retval;

   if( mType == "-" )
   {
      return retval;
   }

   Q_ASSERT( mParameterNames.size() == mDefaultValues.size() );

   retval << "void Settings::setValue( " + enumTypeName() + " id, "+ mTypeName + "value )\n{";

   if( mTopPart.startsWith("@") )
   {
      retval << "\tQSettings *settings = cpSettings->get();";
   }
   else
   {
      retval << "\tQSettings *settings = cpSettings->get( \"" + mTopPart + "\" );";
   }

   entryFormat.append( "\"" );
   if( mParts.size() > 0 )
   {
      foreach( const QString &part, mParts )
      {
         if( part.startsWith( "@" ) )
         {
            entryFormat.append( "\" + QApplication::applicationName + \"" );
         }
         else
         {
            entryFormat.append( part );
         }
         entryFormat.append( "/" );
      }
   }
   entryFormat.append( "%1\"" );

   retval << "\tswitch( id )"
          << "\t{";

   for( int i = 0; i < mParameterNames.size(); i++ )
   {
      retval << "\tcase " + enumTypeName(i) + ":"
             << "\t\tsettings->setValue( " + entryFormat.arg( mParameterNames.at(i) ) + ", value );"
             << "\t\treturn;";
   }

   retval << "\tdefault:"
          << "\t\tqFatal( \"illegal " + enumTypeName() + " value\" );"
          << "\t}"
          << "}\n\n";

   QString defaultValue;
   QString toType( mType );
   if( mType.startsWith( "Q", Qt::CaseInsensitive ) )
   {
      toType = mType.mid( 1 );
   }
   toType.replace( "uint", "UInt" );
   toType.replace( "int", "Int" );
   toType.replace( "ulonglong", "ULongLong" );
   toType.replace( "ulong", "ULong" );
   toType.replace( "long", "Long" );
   toType.replace( "bool", "Bool" );
   toType.replace( "double", "Double" );
   toType.replace( "PoInt", "Point" );
   toType.prepend( "to" );

   retval << mType + " Settings::value( " + enumTypeName() + " id )\n{";

   if( mTopPart.startsWith("@") )
   {
      retval << "\tQSettings *settings = cpSettings->get();";
   }
   else
   {
      retval << "\tQSettings *settings = cpSettings->get( \"" + mTopPart + "\" );";
   }

   retval << "\tswitch( id )"
          << "\t{";

   for( int i = 0; i < mParameterNames.size(); i++ )
   {
      if( mType.endsWith("String") )
      {
         defaultValue = ", \"" + mDefaultValues.at(i) + "\"";
      }
      else
      {
         defaultValue = ", " + mDefaultValues.at(i);
      }
      if( mDefaultValues.at(i) == "@none" )
      {
         defaultValue = "";
      }
      if( mDefaultValues.at(i).startsWith("@code ") )
      {
         defaultValue = ", " + mDefaultValues.at(i).mid(6);
      }

      retval << "\tcase " + enumTypeName( i ) + ":"
             << "\t\treturn settings->value( " + entryFormat.arg( mParameterNames.at(i) ) + defaultValue +" )." + toType + "();";
   }

   retval << "\tdefault:"
          << "\t\tqFatal( \"illegal " + enumTypeName() + " value\" );"
          << "\t\treturn " + mType + "();"
          << "\t}"
          << "}\n\n";

   return retval;
}


QStringList SettingsGeneratorStorage::cleanup() const
{
   QStringList retval;
   if( ( mType != "-" ) || mDeleteNames.isEmpty() )
   {
      return retval;
   }

   QString prefix( mParts.join( "/" ) );
   if( !prefix.isEmpty() )
   {
      prefix.append( ("/") );
   }

   foreach( const QString &name, mDeleteNames )
   {
      if( mTopPart.startsWith("@") )
      {
         retval << "\tremove( \"" + prefix + name + "\" );";
      }
      else
      {
         retval << "\tremove( \"" + prefix + name + "\", \"" + mTopPart + "\" );";
      }
   }

   return retval;
}
