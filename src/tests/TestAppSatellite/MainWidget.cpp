/**
 * MainWidget.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 */

#include "MainWidget.hpp"

#include <QtGui>

#include <SLARTCom.hpp>
#include <SLARTComServer.hpp>


MainWidget::MainWidget( QWidget *parent, Qt::WindowFlags flags )
: QWidget( parent, flags )
, mpSLARTCom( new SLARTCom( 24222, this ) )
#if SLARTCOM_DEBUG
, mpDebugBuffer( new QListWidget( this ) )
#else
, mpDebugBuffer( 0 )
#endif
, mpMessageBuffer( new QListWidget( this ) )
, mpInput( new QLineEdit( this ) )
{
   QVBoxLayout *mainLayout   = new QVBoxLayout( this );
#if QT_VERSION < 0x040300
   mainLayout->setMargin( 3 );
#else
   mainLayout->setContentsMargins( 3, 3, 3, 3 );
#endif
   
#if SLARTCOM_DEBUG
   connect( mpSLARTCom, SIGNAL(debug(const QString&)),
            this, SLOT(addDebug(const QString&)) );
   mainLayout->addWidget( mpDebugBuffer );
   mainLayout->setStretchFactor( mpDebugBuffer, 4 );
#endif
   
   connect( mpSLARTCom, SIGNAL(received(const QByteArray&)),
            this, SLOT(addMessage(const QByteArray&)) );
   mainLayout->addWidget( mpMessageBuffer );
   mainLayout->setStretchFactor( mpMessageBuffer, 1 );
   
   connect( mpInput, SIGNAL(returnPressed()),
            this, SLOT(handleInput()) );
   mainLayout->addWidget( mpInput );
   
   connect( this, SIGNAL(sendText(const QByteArray&)),
            mpSLARTCom, SLOT(send(const QByteArray&)) );
   
   setLayout( mainLayout );
   
   mpSLARTCom->start();
}


void MainWidget::handleInput()
{
   emit sendText( mpInput->text().toUtf8() );
   mpInput->clear();
}


void MainWidget::addDebug( const QString &message )
{
#if SLARTCOM_DEBUG
   addMessage( message.toUtf8(), mpDebugBuffer );
#else
   Q_UNUSED( message );
#endif
}


void MainWidget::addMessage( const QByteArray &message, QListWidget *list )
{
   if( !list )
   {
      list = mpMessageBuffer;
   }
   
   list->addItem( QString::fromUtf8(message) );
      
   while( list->count() > 500 )
   {
      QListWidgetItem *item = list->takeItem(0);
      if( item )
      {
         delete item;
      }
   }
      
   list->scrollToBottom();
}
