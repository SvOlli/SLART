/**
 * MainWidget.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 */

#include "MainWidget.hpp"

#include <QtGui>

#include "SLARTSock.hpp"


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
   connect( mpSLARTSock, SIGNAL(debug(const QString&)),
            this, SLOT(addMessage(const QString&)) );
   connect( this, SIGNAL(sendText(const QString&)),
            mpSLARTSock, SLOT(send(const QString&)) );
   
   setLayout( mainLayout );
   
   mpSLARTSock->init();
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
