/**
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


MySettings::MySettings()
: QSettings( QApplication::organizationName(), QApplication::applicationName() )
{
}


MySettings::MySettings( const QString &application )
: QSettings( QApplication::organizationName(), application )
{
}


void MySettings::setMainWindow( QMainWindow *mainWindow )
{
   QPoint defaultPos( -1, -1 );
   QSize  defaultSize( -1, -1 );
   QPoint pos  = value( "MainWindowPosition", defaultPos ).toPoint();
   QSize  size = value( "MainWindowSize",    defaultSize ).toSize();

   if( size != defaultSize )
   {
      mainWindow->resize( size );
   }
   if( pos != defaultPos )
   {
      mainWindow->move( pos );
   }
}


void MySettings::saveMainWindow( QMainWindow *mainWindow )
{
   setValue( "MainWindowPosition", mainWindow->pos() );
   setValue( "MainWindowSize",    mainWindow->size() );
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
