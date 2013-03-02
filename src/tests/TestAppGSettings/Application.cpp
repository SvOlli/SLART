/*
 * src/tests/TestAppXxtea/Application.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

/* system headers */
#include <cstdio>

/* Qt headers */
#include <QByteArray>
#include <QCoreApplication>
#include <QCryptographicHash>
#include <QFile>
#include <QFileInfo>
#include <QSettings>
#include <QStringList>
#include <QTextStream>

/* local library headers */
#include <GSettingsWrapper.hpp>

/* local headers */


int main( int argc, char *argv[] )
{
   int retval = 0;

   QCoreApplication::setOrganizationName("SLART");
   QCoreApplication::setOrganizationDomain("svolli.org");
   QCoreApplication::setApplicationName("TestAppGSettings");
   QCoreApplication app( argc, argv );

   QTextStream qStdOut( stdout );
   QTextStream qStdErr( stderr );

   GSettingsWrapper gsettingsProxyHttp( "org.gnome.system.proxy.http" );
   {
      bool isValid = gsettingsProxyHttp.isValid();
      bool enabled = gsettingsProxyHttp.valueBool( "enabled" );
      QString host = gsettingsProxyHttp.valueString( "host" );
      int  port    = gsettingsProxyHttp.valueInt( "port" );
      bool useauth = gsettingsProxyHttp.valueBool( "use-authentication" );
      QString user = gsettingsProxyHttp.valueString( "authentication-user" );
      QString pass = gsettingsProxyHttp.valueString( "authentication-password" );

      qStdOut << "\nGSettings Proxy http configuration:"
                 "\nisValid(): " << isValid;
      qStdOut << "\nenabled: " << enabled;
      qStdOut << "\nhost: " << host;
      qStdOut << "\nport: " << port;
      qStdOut << "\nuse-authentication: " << useauth;
      qStdOut << "\nauthentication-user: " << user;
      qStdOut << "\nauthentication-password: " << pass;
      qStdOut << "\n";
   }

   GSettingsWrapper gsettingsProxy( "org.gnome.system.proxy" );
   {
      bool isValid = gsettingsProxy.isValid();
      QString mode = gsettingsProxy.valueString( "mode" );
      QStringList ignoreHosts = gsettingsProxy.valueStringList( "ignore-hosts" );
      qStdOut << "\nGSettings Proxy configuration:"
                 "\nisValid(): " << isValid;
      qStdOut << "\nmode: " << mode;
      qStdOut << "\nignore-hosts: " << ignoreHosts.join(",");
      qStdOut << "\n";
   }

   GSettingsWrapper gsettingsPanel( "com.canonical.Unity.Panel" );
   {
      bool isValid = gsettingsPanel.isValid();
      QStringList systrayWhitelist( gsettingsPanel.valueStringList( "systray-whitelist" ) );
      qStdOut << "\nGSettings Panel configuration:"
                 "\nisValid(): " << isValid;
      qStdOut << "\nsystray-whitelist:" << systrayWhitelist.join(",");
      qStdOut << "\n";
   }

   qStdOut << "\n";

   return retval;
}
