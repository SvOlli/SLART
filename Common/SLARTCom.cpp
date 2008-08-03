/**
 * SLARTCom.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#include "SLARTCom.hpp"
#include "MySettings.hpp"

#include "Trace.hpp"

#include <QMessageBox>
#include <QApplication>
#include <QTimer>


SLARTCom::SLARTCom( QObject *parent )
: QObject( parent )
, mpParent( parent )
, mUdpSocket()
, mPingPort( 0 )
{
   connect( &mUdpSocket, SIGNAL(readyRead()),
            this, SLOT(handleReadyRead()) );
}


void SLARTCom::resetReceiver()
{
   MySettings settings;
   
   if( mUdpSocket.isValid() )
   {
      mUdpSocket.abort();
   }
   
   if( settings.value( "SLARTCommunication", false ).toBool() )
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


bool SLARTCom::ping( const QString &application )
{
   bool active = MySettings().value( "SLARTCommunication", false ).toBool();
   int port = MySettings( application ).value( "UDPListenerPort", 0 ).toInt();
   
   if( (!active) || (port < 1) || (port > 65535) )
   {
      return false;
   }
   
   mPingPort = port;
   mUdpSocket.writeDatagram( QByteArray("PNG"), 
                             QHostAddress::LocalHost, port );
   
   if( port == mPingPort )
   {
      QTimer::singleShot(1000, this, SLOT(handleReadyRead()));
      while( port == mPingPort )
      {
         QApplication::processEvents();
      }
   }
   
   return mPingPort != 0;
}


void SLARTCom::handleReadyRead()
{
   if( !mUdpSocket.hasPendingDatagrams() )
   {
      mPingPort = 0;
   }
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
         
         if( src.at(0) == "PNG" )
         {
            QString data("png");
            
            mUdpSocket.writeDatagram( data.toUtf8(), 
                                      QHostAddress::LocalHost, senderPort );
            return;
         }
         
         if( src.at(0) == "png" )
         {
            mPingPort = ( mPingPort == senderPort ) ? -1 : 0;
            return;
         }
         
         emit packageRead( src );
      }
   }
}
