/**
 * src/tests/TestAppSatellite/MainWidget.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 */

/* base class */
#include "MainWidget.hpp"

/* system headers */

/* Qt headers */
#include <QtGui>

/* local library headers */
#include <Satellite.hpp>
//#include <SatelliteServer.hpp>

/* local headers */


MainWidget::MainWidget( QWidget *parent, Qt::WindowFlags flags )
: QWidget( parent, flags )
, mpSatellite( Satellite::get() )
#if SLARTCOM_DEBUG
, mpDebugBuffer( new QListWidget( this ) )
#else
, mpDebugBuffer( 0 )
#endif
, mpMessageBuffer( new QListWidget( this ) )
, mpInput( new QLineEdit( this ) )
{
   QVBoxLayout *mainLayout   = new QVBoxLayout( this );
   mainLayout->setContentsMargins( 3, 3, 3, 3 );

#if SLARTCOM_DEBUG
   connect( mpSatellite, SIGNAL(debug(const QString&)),
            this, SLOT(addDebug(const QString&)) );
   mainLayout->addWidget( mpDebugBuffer );
   mainLayout->setStretchFactor( mpDebugBuffer, 4 );
#endif

   connect( mpSatellite, SIGNAL(received(const QByteArray&)),
            this, SLOT(addMessage(const QByteArray&)) );
   mainLayout->addWidget( mpMessageBuffer );
   mainLayout->setStretchFactor( mpMessageBuffer, 1 );

   connect( mpInput, SIGNAL(returnPressed()),
            this, SLOT(handleInput()) );
   mainLayout->addWidget( mpInput );

   connect( this, SIGNAL(sendText(const QByteArray&)),
            mpSatellite, SLOT(send(const QByteArray&)) );

   setLayout( mainLayout );

   mpSatellite->setTestApp( true );
   mpSatellite->restart();
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

   list->addItem( QString::fromUtf8(message.constData()) );

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
