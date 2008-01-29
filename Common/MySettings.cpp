/**
 * MySettings.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#include "MySettings.hpp"
#include <QApplication>
#include <QUdpSocket>
#include <QMainWindow>

#include "Trace.hpp"

#ifdef STANDALONE
/* include the version of settings for standalone applications */
#include "MySettingsStandalone.cpp"
#else

MySettings::MySettings()
: QSettings( QApplication::organizationName(), "SLAT" )
{
   beginGroup( QApplication::applicationName() );
}


void MySettings::setGlobalValue( const QString &key, const QVariant &value,
                                 const QString &application )
{
   endGroup();
   if( !application.isEmpty() )
   {
      beginGroup( application );
   }
   setValue( key, value );
   if( !application.isEmpty() )
   {
      endGroup();
   }
   beginGroup( QApplication::applicationName() );
}


QVariant MySettings::globalValue( const QString &key, const QVariant &defaultValue,
                                  const QString &application )
{
   endGroup();
   if( !application.isEmpty() )
   {
      beginGroup( application );
   }
   QVariant retval( value( key, defaultValue ) );
   if( !application.isEmpty() )
   {
      endGroup();
   }
   beginGroup( QApplication::applicationName() );
   return retval;
}


void MySettings::sendUdpMessage( const QByteArray &data, int port )
{
   endGroup();
   bool active = value( "SLATCommunication", false ).toBool();
   beginGroup( QApplication::applicationName() );
   if( !port )
   {
      port = value( "UDPListenerPort", 0 ).toInt();
   }
   
   if( (!active) || (port < 1) || (port > 65535) )
   {
      return;
   }
   QUdpSocket().writeDatagram( data, QHostAddress::LocalHost, port );
}


void MySettings::sendUdpMessage( const QByteArray &data, const QString &application )
{
   endGroup();
   beginGroup( application );
   int port = value( "UDPListenerPort", 0 ).toInt();
   endGroup();
   beginGroup( QApplication::applicationName() );
   
   if( (port < 1) || (port > 65535) )
   {
      return;
   }
   sendUdpMessage( data, port );
}
#endif


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
