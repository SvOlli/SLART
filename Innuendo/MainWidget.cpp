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

#include "Trace.hpp"


MainWidget::MainWidget( QWidget *parent , Qt::WindowFlags flags )
: QWidget( parent, flags )
, mpMessageBuffer( new QListWidget( this ) )
, mpSettingsButton( new QPushButton( tr("Settings"), this ) )
, mpBufferSizeLabel( new QLabel( tr("Buffer Size:"), this ) )
, mpBufferSize( new QSpinBox( this ) )
, mpConfig( new ConfigDialog( this ) )
, mBufferSize(500)
, mSLATCom()
{
   QGridLayout *mainLayout   = new QGridLayout( this );

   mpBufferSizeLabel->setAlignment( Qt::AlignRight | Qt::AlignVCenter );
   mpBufferSize->setRange( 50, 50000 );
   
   mainLayout->addWidget( mpMessageBuffer,   0, 0, 1, 3 );
   mainLayout->addWidget( mpSettingsButton,  1, 0 );
   mainLayout->addWidget( mpBufferSizeLabel, 1, 1 );
   mainLayout->addWidget( mpBufferSize,      1, 2 );

   connect( mpSettingsButton, SIGNAL(clicked()),
            this, SLOT(handleSettings()) );
   connect( mpBufferSize, SIGNAL(valueChanged(int)),
            this, SLOT(setBufferSize(int)) );
   connect( &mSLATCom, SIGNAL(packageRead(QStringList)),
            this, SLOT(handleSLAT(QStringList)) );
   connect( &mSLATCom, SIGNAL(updateConfig()),
            mpConfig, SLOT(readSettings()) );

   readConfig();
   
   setLayout( mainLayout );
}


void MainWidget::readConfig()
{
   MySettings settings;

   mSLATCom.resetReceiver();

   mpBufferSize->setValue( mBufferSize );
}


void MainWidget::setBufferSize( int size )
{
   mBufferSize = size;
}


void MainWidget::handleSettings()
{
   mpConfig->readSettings();
   mpConfig->exec();
}


void MainWidget::handleSLAT( const QStringList &message )
{
   mpMessageBuffer->addItem( ">" );

   for( int i = 0; i < message.size(); i++ )
   {
      mpMessageBuffer->addItem( message.at(i) );
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
