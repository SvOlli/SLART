/**
 * MainWidget.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 */

#include "MainWidget.hpp"

#include <QtGui>
#include "MySettings.hpp"
#include "ConfigDialog.hpp"


MainWidget::MainWidget( QWidget *parent , Qt::WindowFlags flags )
: QWidget( parent, flags )
, mpMessageBuffer( new QListWidget( this ) )
, mpSettingsButton( new QPushButton( tr("Settings"), this ) )
, mpBufferSizeLabel( new QLabel( tr("Buffer Size"), this ) )
, mpBufferSize( new QSpinBox( this ) )
, mpConfig( new ConfigDialog( this ) )
, mBufferSize(500)
, mUdpSocket()
{
   QGridLayout *mainLayout   = new QGridLayout( this );

   mpBufferSize->setRange( 50, 50000 );
   
   mainLayout->addWidget( mpMessageBuffer,   0, 0, 1, 3 );
   mainLayout->addWidget( mpSettingsButton,  1, 0 );
   mainLayout->addWidget( mpBufferSizeLabel, 1, 1 );
   mainLayout->addWidget( mpBufferSize,      1, 2 );

   connect( mpSettingsButton, SIGNAL(clicked()),
            this, SLOT(handleSettings()) );
   connect( mpBufferSize, SIGNAL(valueChanged(int)),
            this, SLOT(setBufferSize(int)) );
   connect( &mUdpSocket, SIGNAL(readyRead()),
            this, SLOT(handleUdp()) );

   readConfig();
   
   setLayout( mainLayout );
}


void MainWidget::readConfig()
{
   MySettings settings;

   if( settings.globalValue( "SLATCommunication", false ).toBool() )
   {
      int port = settings.globalValue( "UDPListenerPort", 24222 ).toInt();
      bool success = mUdpSocket.bind( QHostAddress::LocalHost, port,
                                      QUdpSocket::ShareAddress |
                                      QUdpSocket::ReuseAddressHint );
      if( !success )
      {
         QString message( tr("Could not listen on port ") );
         message.append( QString::number( port ) );
         QMessageBox::critical( this, QApplication::applicationName(), message );
      }
   }

   mpBufferSize->setValue( mBufferSize );
}


void MainWidget::setBufferSize( int size )
{
   mBufferSize = size;
}


void MainWidget::handleSettings()
{
   mpConfig->exec();
}


void MainWidget::handleUdp()
{
   int i;

   while (mUdpSocket.hasPendingDatagrams())
   {
      QByteArray datagram;
      datagram.resize( mUdpSocket.pendingDatagramSize() );
      QHostAddress senderHost;
      quint16 senderPort;

      mUdpSocket.readDatagram( datagram.data(), datagram.size(),
                               &senderHost, &senderPort );

      QString header( ">>>>> From: " );
      header.append( senderHost.toString() );
      header.append( ":" );
      header.append( QString::number(senderPort) );
      header.append( " <<<<<" );
      mpMessageBuffer->addItem( header );
      
      QStringList src( QString::fromUtf8( datagram.data() ).remove('\r').split("\n") );

      for( i = 0; i < src.size(); i++ )
      {
         mpMessageBuffer->addItem( src.at(i) );
      }

      while( mpMessageBuffer->count() > mBufferSize )
      {
         QListWidgetItem *item = mpMessageBuffer->takeItem(0);
       if( item )
       {
          delete item;
       }
      }
      
      mpMessageBuffer->scrollToBottom();
   }
}
