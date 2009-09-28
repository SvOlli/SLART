/**
 * MainWidget.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 */

#include "MainWidget.hpp"

#include <QtGui>

#include "SLARTSock.hpp"
#include "SLARTSockServer.hpp"


MainWidget::MainWidget( QWidget *parent , Qt::WindowFlags flags )
: QWidget( parent, flags )
, mpSLARTSock( new SLARTSock( this ) )
, mpMessageBuffer( new QListWidget( this ) )
, mpInput( new QLineEdit( this ) )
{
   QVBoxLayout *mainLayout   = new QVBoxLayout( this );
#if QT_VERSION < 0x040300
   mainLayout->setMargin( 3 );
#else
   mainLayout->setContentsMargins( 3, 3, 3, 3 );
#endif
   mainLayout->addWidget( mpMessageBuffer );
   mainLayout->addWidget( mpInput );
   
   connect( mpInput, SIGNAL(returnPressed()),
            this, SLOT(handleInput()) );
#ifdef SLARTSOCK_DEBUG
   connect( mpSLARTSock, SIGNAL(debug(const QString&)),
            this, SLOT(addMessage(const QString&)) );
#endif
   connect( mpSLARTSock, SIGNAL(received(const QString&)),
            this, SLOT(addMessage(const QString&)) );
   connect( this, SIGNAL(sendText(const QString&)),
            mpSLARTSock, SLOT(send(const QString&)) );
   
   setLayout( mainLayout );

#if 0
   SLARTSockServer *server = new SLARTSockServer( QDir::home().absolutePath() + "/.SLART.sock", this );
   connect( server, SIGNAL(debug(const QString&)),
            this, SLOT(addMessage(const QString&)) );
   server->listen();
#endif
   mpSLARTSock->start();
}


void MainWidget::handleInput()
{
   emit sendText( mpInput->text() );
   mpInput->clear();
}


void MainWidget::addMessage( const QString &message )
{
   mpMessageBuffer->addItem( message );
      
   while( mpMessageBuffer->count() > 500 )
   {
      QListWidgetItem *item = mpMessageBuffer->takeItem(0);
      if( item )
      {
         delete item;
      }
   }
      
   mpMessageBuffer->scrollToBottom();
}
