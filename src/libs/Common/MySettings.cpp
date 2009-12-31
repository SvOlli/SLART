/**
 * src/libs/Common/MySettings.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
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


void MySettings::sendUdpMessage( const QString &data, int port )
{
#if 0
TRACESTART(MySettings::sendUdpMessage)
#endif
   bool active = value( "SLARTCommunication", false ).toBool();
   if( !port )
   {
      port = value( "UDPListenerPort", 0 ).toInt();
   }
   
#if 0
TRACEMSG << active << port << data;
#endif
   if( (!active) || (port < 1) || (port > 65535) )
   {
      return;
   }
   QUdpSocket().writeDatagram( data.toUtf8(), QHostAddress::LocalHost, port );
}


void MySettings::sendUdpMessage( const QString &data, const QString &application )
{
   int port = QSettings( QApplication::organizationName(), application )
                        .value( "UDPListenerPort", 0 ).toInt();
   
   if( (port < 1) || (port > 65535) )
   {
      return;
   }
   sendUdpMessage( data, port );
}


void MySettings::sendNotification( const QString &data )
{
   int i;
   
   if( value( "SLARTCommunication", false ).toBool() )
   {
      QList<int>  listenerPorts;
      
      beginGroup( "Listeners" );
      QStringList listenerNames( allKeys() );
      for( i = 0; i < listenerNames.count(); i++ )
      {
         int port = value( listenerNames.at(i), 0 ).toInt();
         if( (port > 0) && (port <= 65535) )
         {
            listenerPorts << port;
         }
      }
      endGroup();
      
      for( i = 0; i < listenerPorts.count(); i++ )
      {
         sendUdpMessage( data, listenerPorts.at(i) );
      }
   }
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