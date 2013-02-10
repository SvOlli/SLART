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


int main(int argc, char *argv[])
{
   int retval = 0;

   QCoreApplication::setOrganizationName("SLART");
   QCoreApplication::setOrganizationDomain("svolli.org");
   QCoreApplication::setApplicationName("TestAppXxtea");
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

      qStdOut << "GSettings Proxy http configuration:\n";
      qStdOut << "isValid(): " << isValid << "\n";
      qStdOut << "enabled: " << enabled << "\n";
      qStdOut << "host: " << host << "\n";
      qStdOut << "port: " << port << "\n";
      qStdOut << "use-authentication: " << useauth << "\n";
      qStdOut << "authentication-user: " << user << "\n";
      qStdOut << "authentication-password: " << pass << "\n";
      qStdOut << "\n";
   }

   GSettingsWrapper gsettingsProxy( "org.gnome.system.proxy" );
   {
      bool isValid = gsettingsProxyHttp.isValid();
      QStringList ignoreHosts = gsettingsProxy.valueStringList( "ignore-hosts" );
      qStdOut << "GSettings Proxy configuration:\n";
      qStdOut << "isValid(): " << isValid << "\n";
      qStdOut << "ignore-hosts: " << ignoreHosts.join(",") << "\n";
      qStdOut << "\n";
   }

   GSettingsWrapper gsettingsPanel( "com.canonical.Unity.Panel" );
   {
      bool isValid = gsettingsPanel.isValid();
      QStringList systrayWhitelist( gsettingsPanel.valueStringList( "systray-whitelist" ) );
      qStdOut << "GSettings Panel configuration:\n";
      qStdOut << "isValid(): " << isValid << "\n";
      qStdOut << "systray-whitelist:" << systrayWhitelist.join(",") << "\n";
      qStdOut << "\n";
   }
   return retval;
}
