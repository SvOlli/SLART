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
#include <MagicEncoderProxy.hpp>

/* local headers */
#include "Database.hpp"
#include "MySettings.hpp"
#include "Satellite.hpp"
#include "Version.hpp"

#include <QtDebug>

MagicEncoderList MagicEncoderLoader::tryLoading( const QString &msgHeader,
                                                 Satellite *satellite,
                                                 QApplication *app )
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
         encoderList = tryLoading( msgHeader, searchDir, satellite );
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
         encoderList = tryLoading( msgHeader, searchDir, satellite );
         if( encoderList.size() > 0 )
         {
            return encoderList;
         }
      }
   }
#endif
   return tryLoading( msgHeader, pluginsDir, satellite );
}


MagicEncoderList MagicEncoderLoader::tryLoading( const QString &msgHeader,
                                                 const QDir &dir,
                                                 Satellite *satellite )
{
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
   for( int i = 0; i < encoderNames.size(); i++ )
   {
      QString encoderName( dir.absoluteFilePath( encoderNames.at(i) ) );
      MagicEncoderProxy *encoder = new MagicEncoderProxy( satellite );

      if( encoder->pluginLoad( encoderName, msgHeader ) )
      {
         encoderList.append( encoder );
      }
      else
      {
         Satellite::send1( QObject::tr("I0L\nerror loading %1:\n%2")
                           .arg( encoderName, encoder->pluginLoadErrorString() ).toUtf8() );
         delete encoder;
      }
   }
   return encoderList;
}
