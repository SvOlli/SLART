/**
 * src/tools/SettingsGenerator/SettingsGeneratorHandler.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

/* class declaration */
#include "SettingsGeneratorHandler.hpp"

/* system headers */

/* Qt headers */
#include <QtDebug>
#include <QCoreApplication>
#include <QSettings>
#include <QStringList>

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */

/* forward declaration of local classes */


SettingsGeneratorHandler::SettingsGeneratorHandler( const QString &fileName )
: mSpaces( 3 )
, mFileName( fileName )
, mHppFileName()
, mCppFileName()
, mIncludes()
, mStorageList()
{
}


SettingsGeneratorHandler::~SettingsGeneratorHandler()
{
}


void SettingsGeneratorHandler::read()
{
   QSettings settings( mFileName, QSettings::IniFormat );
   QStringList appNames( settings.childGroups() );
   mHppFileName = settings.value( "hppFileName" ).toString();
   mCppFileName = settings.value( "cppFileName" ).toString();
   mIncludes    = settings.value( "includes" ).toStringList();
   foreach( const QString &name, appNames )
   {
      settings.beginGroup( name );
      foreach( const QString &type, settings.childGroups() )
      {
         SettingsGeneratorStorage storage( name, type );

         settings.beginGroup( type );
         foreach( const QString &key, settings.childKeys() )
         {
            storage.addParameter( key, settings.value( key ).toString() );
         }
         settings.endGroup();

         mStorageList << storage;
      }
      settings.endGroup();
   }
}


void SettingsGeneratorHandler::setIndention( int spaces )
{
   mSpaces = spaces;
}


QString SettingsGeneratorHandler::enums( int start ) const
{
   QStringList retval;
   foreach( const SettingsGeneratorStorage &storage, mStorageList )
   {
      retval << storage.enums();
   }
   retval.replaceInStrings( QRegExp("^"), QString( start * mSpaces, QChar(' ') ) );
   retval.replaceInStrings( "\t", QString( mSpaces, QChar(' ') ) );
   return retval.join( "\n" );
}


QString SettingsGeneratorHandler::declarations( int start ) const
{
   QStringList retval;
   foreach( const SettingsGeneratorStorage &storage, mStorageList )
   {
      retval << storage.declaration();
   }
   retval << "/*!"
          << " \\brief cleanup unused registry entries\n"
          << " automatically generated"
          << " */"
          << "void cleanup();\n";

   retval.replaceInStrings( QRegExp("^"), QString( start * mSpaces, QChar(' ') ) );
   retval.replaceInStrings( "\t", QString( mSpaces, QChar(' ') ) );
   return retval.join( "\n" );
}


QString SettingsGeneratorHandler::definitions() const
{
   QStringList retval;
   foreach( const SettingsGeneratorStorage &storage, mStorageList )
   {
      retval << storage.definition();
   }
   retval << "void Settings::cleanup()"
          << "{";
   foreach( const SettingsGeneratorStorage &storage, mStorageList )
   {
      retval << storage.cleanup();
   }
   retval << "}\n";
   retval.replaceInStrings( "\t", QString( mSpaces, QChar(' ') ) );
   return retval.join( "\n" );
}


QString SettingsGeneratorHandler::includes() const
{
   QStringList retval;
   foreach( const QString &include, mIncludes )
   {
      retval << "#include " + include;
   }
   retval << "\n\n";
   return retval.join( "\n" );
}


QString SettingsGeneratorHandler::hppFileName() const
{
   return mHppFileName;
}


QString SettingsGeneratorHandler::cppFileName() const
{
   return mCppFileName;
}
