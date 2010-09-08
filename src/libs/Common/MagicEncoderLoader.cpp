/**
 * src/libs/Common/MagicEncoderLoader.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

/* class declaration */
#include "MagicEncoderLoader.hpp"

/* system headers */
#include <cstdlib> // exit(), getenv()

/* Qt headers */
#include <QApplication>
#include <QDir>
#include <QMessageBox>
#include <QPluginLoader>
#include <QStringList>

/* local library headers */
#include "../../libs/Magic/MagicEncoderInterface.hpp"
#include "../../libs/Magic/MagicEncoder/MagicEncoder.hpp"

/* local headers */
#include "Database.hpp"
#include "MySettings.hpp"
#include "Satellite.hpp"
#include "Version.hpp"

#include <QtDebug>

MagicEncoderList MagicEncoderLoader::tryLoading( QApplication *app )
{
   MagicEncoderList encoderList;
   if( !app )
   {
      app = qApp;
   }
   QDir pluginsDir( app->applicationDirPath() );
#if defined Q_OS_MAC
   if( pluginsDir.dirName() == "MacOS" )
   {
      QDir searchDir( pluginsDir );
      searchDir.cdUp();
      if( searchDir.cd( "PlugIns" ) )
      {
         encoderList = tryLoading( searchDir );
         if( encoderList.size() > 0 )
         {
            return encoderList;
         }
      }
   }
#elif defined Q_OS_UNIX
   if( pluginsDir.dirName() == "bin" )
   {
      QDir searchDir( pluginsDir );
      searchDir.cdUp();
      if( searchDir.cd( "lib" ) )
      {
         encoderList = tryLoading( searchDir );
         if( encoderList.size() > 0 )
         {
            return encoderList;
         }
      }
   }
#endif
   return tryLoading( pluginsDir );
}


MagicEncoderList MagicEncoderLoader::tryLoading( const QDir &dir )
{
   Satellite *satellite = Satellite::get();
   QStringList nameFilter;
   MagicEncoderList encoderList;
#if defined Q_OS_WIN
   nameFilter << "MagicEncoder*.dll";
#elif defined Q_OS_MAC
   nameFilter << "libMagicEncoder*.dylib";
#else
   nameFilter << "libMagicEncoder*.so";
#endif
   QStringList encoderNames( dir.entryList( nameFilter, QDir::Files | QDir::Readable, QDir::Name ) );
   QPluginLoader pluginLoader;
   for( int i = 0; i < encoderNames.size(); i++ )
   {
      QString encoderName( dir.absoluteFilePath( encoderNames.at(i) ) );
      pluginLoader.setFileName( encoderName );
      QObject *plugin = pluginLoader.instance();
      MagicEncoderInterface *encoder = qobject_cast<MagicEncoderInterface*>(plugin);
      if( encoder )
      {
         encoder->setup( satellite, encoderName );
         encoderList.append( encoder );
      }
      else
      {
         satellite->send( QObject::tr("I0L\nerror loading %1:\n%2")
                          .arg( encoderName, pluginLoader.errorString() ).toUtf8() );
         pluginLoader.unload();
      }
   }
   return encoderList;
}
