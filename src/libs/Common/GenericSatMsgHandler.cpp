/**
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
#include "WidgetShot.hpp"


GenericSatMsgHandler::GenericSatMsgHandler( Satellite *satellite, StartupMode mode )
: QObject( satellite )
, mpSatellite( satellite )
, mWithQuitDialog( mode == WithPingAndDialog )
{
   connect( mpSatellite, SIGNAL(received(const QByteArray &)),
            this, SLOT(handle(const QByteArray &)) );
   connect( this, SIGNAL(reply(const QByteArray &)),
            mpSatellite, SLOT(send(const QByteArray &)) );
   if( mode != WithoutPing )
   {
      connect( mpSatellite, SIGNAL(connected()),
               this, SLOT(sendPing()) );
   }
}


GenericSatMsgHandler::~GenericSatMsgHandler()
{
}


void GenericSatMsgHandler::sendPing()
{
   disconnect( mpSatellite, SIGNAL(connected()),
               this, SLOT(sendPing()) );
   emit reply( QByteArray("PNG\n") + QApplication::applicationName().toUtf8() );
}


void GenericSatMsgHandler::handle( const QByteArray &msg )
{
   QStringList src( Satellite::split( msg ) );

   if( src.size() > 0 )
   {
      if( src.at(0) == "CFG" )
      {
         mpSatellite->restart();
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
