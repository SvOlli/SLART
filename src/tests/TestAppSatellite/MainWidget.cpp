/*
 * src/tests/TestAppSatellite/MainWidget.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

/* base class */
#include "MainWidget.hpp"

/* system headers */

/* Qt headers */
#include <QBoxLayout>
#include <QLineEdit>
#include <QListWidget>
#include <QString>

/* local library headers */
#include <Satellite.hpp>
//#include <SatelliteServer.hpp>

/* local headers */


MainWidget::MainWidget( QWidget *parent, Qt::WindowFlags flags )
: QWidget( parent, flags )
, mpSatellite( Satellite::get() )
#if SATELLITE_DEBUG
, mpDebugBuffer( new QListWidget( this ) )
#else
, mpDebugBuffer( 0 )
#endif
, mpMessageBuffer( new QListWidget( this ) )
, mpInput( new QLineEdit( this ) )
{
   QVBoxLayout *mainLayout   = new QVBoxLayout( this );
   mainLayout->setContentsMargins( 3, 3, 3, 3 );

#if SATELLITE_DEBUG
   connect( mpSatellite, SIGNAL(debug(QByteArray)),
            this, SLOT(addDebug(QByteArray)) );
   mainLayout->addWidget( mpDebugBuffer );
   mainLayout->setStretchFactor( mpDebugBuffer, 4 );
#endif

   connect( mpSatellite, SIGNAL(received(QByteArray)),
            this, SLOT(addMessage(QByteArray)) );
   mainLayout->addWidget( mpMessageBuffer );
   mainLayout->setStretchFactor( mpMessageBuffer, 1 );

   connect( mpInput, SIGNAL(returnPressed()),
            this, SLOT(handleInput()) );
   mainLayout->addWidget( mpInput );

   connect( this, SIGNAL(sendText(QByteArray)),
            mpSatellite, SLOT(send(QByteArray)) );

   setLayout( mainLayout );
}


void MainWidget::handleInput()
{
   emit sendText( mpInput->text().toUtf8() );
   mpInput->clear();
}


void MainWidget::addDebug( const QByteArray &message )
{
#if SATELLITE_DEBUG
   addMessage( message, mpDebugBuffer );
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
