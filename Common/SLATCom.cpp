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
      QStringList listeners( settings.value( "Listeners", QStringList() ).toStringList() );
      updateListeners( &settings, listeners );
      
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
            settings.sync();
            settings.sendUdpMessage( "CFG\n", src.at(1) );
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
            settings.sync();
            settings.sendUdpMessage( "CFG\n", src.at(1) );
            return;
         }
         
         emit packageRead( src );
      }
   }
}


void SLATCom::updateListeners( MySettings *settings, const QStringList &listeners )
{
   if( listeners.count() )
   {
      settings->setValue( "Listeners", listeners );
   }
   else
   {
      settings->remove( "Listeners" );
   }
   
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


void SLATCom::sendNotification( const QString &data )
{
#if 0
TRACESTART(SLATCom::sendNotification)
TRACEMSG << mPorts;
#endif
   MySettings settings;
   
   for( int i = 0; i < mPorts.count(); i++ )
   {
      settings.sendUdpMessage( data, mPorts.at(i) );
   }
}
