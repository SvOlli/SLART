/**
 * src/libs/Common/GenericSatMsgHandler.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 */

#include "GenericSatMsgHandler.hpp"

#include <QApplication>
#include <QByteArray>
#include <QStringList>

#include "Satellite.hpp"
#include "WidgetShot.hpp"


GenericSatMsgHandler::GenericSatMsgHandler( Satellite *satellite )
: QObject( satellite )
, mpSatellite( satellite )
{
   connect( satellite, SIGNAL(received(const QByteArray &)),
            this, SLOT(handle(const QByteArray &)) );
   connect( this, SIGNAL(reply(const QByteArray &)),
            satellite, SLOT(send(const QByteArray &)) );
}

GenericSatMsgHandler::~GenericSatMsgHandler()
{
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
         emit reply( QByteArray("png\n") + QApplication::applicationName().toUtf8() );
         return;
      }

      if( src.at(0) == "png" )
      {
         if( src.size() > 1 )
         {
            if( src.at(1) == QApplication::applicationName() )
            {
               emit anotherInstance();
               return;
            }
         }
      }

      if( src.at(0) == "SHT" )
      {
         if( src.size() > 2 )
         {
            WidgetShot::shootWidget( src.at(1), src.at(2) );
         }
         return;
      }
   }
}
