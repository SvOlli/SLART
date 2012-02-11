/*
 * src/tools/SettingsGenerator/SettingsGenerator.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

/* system headers */

/* Qt headers */
#include <QtDebug>
#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QSettings>
#include <QStringList>

/* local library headers */

/* local headers */
#include "SettingsGeneratorHandler.hpp"

void processType( QSettings *settings, const QString &name, const QString &type )
{
   settings->beginGroup( type );
   foreach( const QString &key, settings->childKeys() )
   {
      qDebug() << name << type << key << settings->value(key).toString();
   }
   settings->endGroup();
}

void processApp( QSettings *settings, const QString &name )
{
   settings->beginGroup( name );
   foreach( const QString &type, settings->childGroups() )
   {
      processType( settings, name, type );
   }
   settings->endGroup();
}

int main( int argc, char *argv[] )
{
   QCoreApplication app( argc, argv );

   QStringList args( app.arguments() );
   qDebug() << args.takeFirst();

   QStringList sourceData;
   QStringList targetData;
   bool useCode = true;
   foreach( const QString &name, args )
   {
      QFileInfo iniFileInfo( name );
      QDir::setCurrent( iniFileInfo.absolutePath() );
      SettingsGeneratorHandler handler( iniFileInfo.fileName() );
      //QSettings ini( iniFileInfo.fileName(), QSettings::IniFormat );

      handler.setIndention( 3 );
      handler.read();
      qDebug() << QDir::currentPath();
      //qDebug() << ini.value( "includes" ).toStringList();

      QFile hpp( handler.hppFileName() );
      hpp.open( QIODevice::ReadOnly );
      sourceData = QString( hpp.readAll() ).split("\n");
      hpp.close();

      foreach( const QString &line, sourceData )
      {
         if( line.contains( "/* generated code end */" ) )
         {
            useCode = true;
         }
         if( useCode )
         {
            targetData << line;
         }
         if( line.contains( "/* generated code start */" ) )
         {
            useCode = false;
            targetData << "";
            targetData << handler.enums( 1 );
            targetData << handler.declarations( 1 );
         }
      }

      hpp.open( QIODevice::WriteOnly );
      hpp.write( targetData.join( "\n" ).toLocal8Bit() );
      hpp.close();
      qDebug() << "wrote:" << hpp.fileName();

      QFile cpp( handler.cppFileName() );
      cpp.open( QIODevice::WriteOnly );
      cpp.write( handler.includes().toLocal8Bit() );
      cpp.write( handler.definitions().toLocal8Bit() );
      cpp.close();
      qDebug() << "wrote:" << cpp.fileName();
   }

   return 0;
}
