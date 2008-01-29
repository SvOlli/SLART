/**
 * SLATCom.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#include "SLATCom.hpp"
#include "MySettings.hpp"

#include "Trace.hpp"

#include <QMessageBox>
#include <QApplication>


SLATCom::SLATCom( QObject *parent )
: QUdpSocket( parent )
, mpParent( parent )
{
   connect( this, SIGNAL(readyRead()),
            this, SLOT(handleReadyRead()) );
}


void SLATCom::startReceiver()
{
   MySettings settings;
   
   if( settings.value( "SLATCommunication", false ).toBool() )
   {
      int port = settings.value( "UDPListenerPort", 0 ).toInt();
      if( (port > 0) && (port <= 65535) )
      {
         if( !bind( QHostAddress::LocalHost, port ) )
         {
            QString message( tr("Could not listen on port ") );
            message.append( QString::number( port ) );
            QMessageBox::critical( 0, QApplication::applicationName(), message );
         }
      }
   }
}


void SLATCom::stopReceiver()
{
   close();
}


void SLATCom::handleReadyRead()
{
   while( hasPendingDatagrams() )
   {
      QByteArray datagram;
      datagram.resize( pendingDatagramSize() );
      QHostAddress senderHost;
      quint16 senderPort;
      
      readDatagram( datagram.data(), datagram.size(),
                    &senderHost, &senderPort );
      
      QStringList src( QString( datagram ).remove('\r').split("\n",QString::SkipEmptyParts) );
      
      if( src.count() > 0 )
      {
         if( src.at(0) == "CFG" )
         {
            emit updateConfig();
            return;
         }
         
         if( src.at(0) == "NF1" )
         {
            if( src.count() < 2 )
            {
               return;
            }
            
            MySettings settings;
            QStringList listeners( settings.value( "Listeners", QStringList() ).toStringList() );
            
            if( listeners.indexOf( src.at(1) ) < 0 )
            {
               listeners.append( src.at(1) );
            }
            
            updateListeners( &settings, listeners );
            return;
         }
         
         if( src.at(0) == "NF0" )
         {
            if( src.count() < 2 )
            {
               return;
            }
            
            MySettings settings;
            QStringList listeners( settings.value( "Listeners", QStringList() ).toStringList() );
            
            listeners.removeAt( listeners.indexOf( src.at(1) ) );
            
            updateListeners( &settings, listeners );
            return;
         }
         
         emit packageRead( src );
      }
   }
}


void SLATCom::updateListeners( MySettings *settings, const QStringList &listeners )
{
   settings->setValue( "Listeners", listeners );
   
   mPorts.clear();
   for( int i = 0; i < listeners.count(); i++ )
   {
      int port = settings->globalValue( "UDPListenerPort", 0, listeners.at(i) ).toInt();
      if( (port > 0) && (port <= 65535) )
      {
         mPorts.append( port );
      }
   }
}
