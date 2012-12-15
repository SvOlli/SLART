/*
 * src/libs/Common/GenericSatMsgHandler.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */


/* class declaration */
#include "GenericSatMsgHandler.hpp"

/* system headers */

/* Qt headers */
#include <QApplication>
#include <QByteArray>
#include <QMainWindow>
#include <QMessageBox>
#include <QStringList>
#include <QTimer>
#include <QWidget>

/* local library headers */

/* local headers */
#include "Satellite.hpp"
#include "SatelliteSingleSend.hpp"
#include "Settings.hpp"
#include "WidgetShot.hpp"


GenericSatMsgHandler::GenericSatMsgHandler( Satellite *satellite, StartupMode mode )
: QObject( satellite )
, mConnectMsg()
, mpSatellite( satellite )
, mWithQuitDialog( mode == WithPingAndDialog )
{
   connect( mpSatellite, SIGNAL(received(QByteArray)),
            this, SLOT(handle(QByteArray)) );
   connect( this, SIGNAL(reply(QByteArray)),
            mpSatellite, SLOT(send(QByteArray)) );
   connect( mpSatellite, SIGNAL(connected()),
            this, SLOT(connected()) );
   if( mode != WithoutPing )
   {
      connect( mpSatellite, SIGNAL(connected()),
               this, SLOT(sendPing()) );
   }
}


GenericSatMsgHandler::~GenericSatMsgHandler()
{
}


void GenericSatMsgHandler::setConnectMsg( const QByteArray &msg )
{
   mConnectMsg = msg;
}


Satellite *GenericSatMsgHandler::createSatellite()
{
   if( Settings::value( Settings::CommonUseSatellite ) )
   {
      return Satellite::create( QHostAddress( Settings::value( Settings::GlobalSatelliteHost ) ),
                                Settings::value( Settings::GlobalSatellitePort ) );
   }
   return 0;
}


void GenericSatMsgHandler::send(const QByteArray &message)
{
   SatelliteSingleSend::send( QHostAddress( Settings::value( Settings::GlobalSatelliteHost ) ),
                              Settings::value( Settings::GlobalSatellitePort ), message );
}


void GenericSatMsgHandler::sendPing()
{
   disconnect( mpSatellite, SIGNAL(connected()),
               this, SLOT(sendPing()) );
   emit reply( QByteArray("PNG\n") + QApplication::applicationName().toUtf8() );
}


void GenericSatMsgHandler::connected()
{
   if( !mConnectMsg.isNull() )
   {
      mpSatellite->send( mConnectMsg );
   }
}


void GenericSatMsgHandler::handle( const QByteArray &msg )
{
   QStringList src( Satellite::split( msg ) );

   if( src.size() > 0 )
   {
      if( src.at(0) == "CFG" )
      {
         emit updateConfig();
         return;
      }

      if( src.at(0) == "PNG" )
      {
         bool shouldEmit = false;
         if( src.size() > 1 )
         {
            if( src.at(1) == QApplication::applicationName() )
            {
               shouldEmit = true;
            }
         }
         else
         {
            shouldEmit = true;
         }

         if( shouldEmit )
         {
            emit reply( QByteArray("png\n") + QApplication::applicationName().toUtf8() );
         }
         return;
      }

      if( src.at(0) == "png" )
      {
         if( src.size() > 1 )
         {
            if( src.at(1) == QApplication::applicationName() )
            {
               emit anotherInstance();
               if( mWithQuitDialog )
               {
                  anotherInstanceMessage();
               }
            }
         }
         return;
      }

      if( src.at(0) == "SHT" )
      {
         if( src.size() > 3 )
         {
            if( src.at(1) == QApplication::applicationName() )
            {
               WidgetShot::shootWidget( src.at(2), src.at(3) );
            }
         }
         return;
      }
   }
}


void GenericSatMsgHandler::anotherInstanceMessage()
{
   mWithQuitDialog = false;
   QMainWindow *mainWindow = 0;
   foreach( QWidget *widget, QApplication::allWidgets() )
   {
      mainWindow = qobject_cast<QMainWindow*>( widget );
      if( mainWindow )
      {
         QMessageBox::critical( mainWindow,
                                QApplication::applicationName(),
                                tr("Another instance of %1 is already running."
                                   "\nQuitting this one.")
                                .arg( QApplication::applicationName() ) );
         QTimer::singleShot( 333, qApp, SLOT(quit()) );
         break;
      }
   }
}
