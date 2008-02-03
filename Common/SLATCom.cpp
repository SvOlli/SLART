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
: QObject( parent )
, mpParent( parent )
, mUdpSocket()
{
   connect( &mUdpSocket, SIGNAL(readyRead()),
            this, SLOT(handleReadyRead()) );
}


void SLATCom::resetReceiver()
{
   MySettings settings;
   
   if( mUdpSocket.isValid() )
   {
      mUdpSocket.abort();
   }
   
   if( settings.value( "SLATCommunication", false ).toBool() )
   {
      int port = settings.value( "UDPListenerPort", 0 ).toInt();
      if( (port > 0) && (port <= 65535) )
      {
         if( !mUdpSocket.bind( QHostAddress::LocalHost, port ) )
         {
            QString message( tr("Could not listen on port ") );
            message.append( QString::number( port ) );
            QMessageBox::critical( 0, QApplication::applicationName(), message );
         }
      }
   }
}


void SLATCom::handleReadyRead()
{
   while( mUdpSocket.hasPendingDatagrams() )
   {
      QByteArray datagram;
      datagram.resize( mUdpSocket.pendingDatagramSize() );
      QHostAddress senderHost;
      quint16 senderPort;
      
      mUdpSocket.readDatagram( datagram.data(), datagram.size(),
                               &senderHost, &senderPort );
      
      QStringList src( QString::fromUtf8( datagram.data() ).remove('\r').split("\n",QString::SkipEmptyParts) );
      
      if( src.count() > 0 )
      {
         if( src.at(0) == "CFG" )
         {
            emit updateConfig();
            return;
         }
         
         emit packageRead( src );
      }
   }
}
