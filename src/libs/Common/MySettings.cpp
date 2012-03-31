/*
 * src/libs/Common/MySettings.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#include "MySettings.hpp"
#include <QApplication>
#include <QUdpSocket>
#include <QMainWindow>

#include "Trace.hpp"


MySettings::MySettings( QObject *parent )
: QSettings( QApplication::organizationName(), QApplication::applicationName(), parent )
{
}


MySettings::MySettings( const QString &application, QObject *parent )
: QSettings( QApplication::organizationName(), application, parent )
{
}


MySettings::~MySettings()
{
}


QString MySettings::styleSheetFile()
{
   if( value( "UseGlobalStyleSheetFile", true ).toBool() )
   {
      return MySettings( "Global" ).value( "StyleSheetFile", QString() ).toString();
   }
   else
   {
      return value( "StyleSheetFile", QString() ).toString();
   }
}
