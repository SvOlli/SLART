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
#include "MyClipboard.hpp"
#include "ExecButton.hpp"

#include "Trace.hpp"


MainWidget::MainWidget( QWidget *parent, Qt::WindowFlags flags )
: QWidget( parent, flags )
, mpMessageBuffer( new QListWidget( this ) )
, mpSettingsButton( new QPushButton( tr("Settings"), this ) )
, mpBufferSizeLabel( new QLabel( tr("Buffer Size:"), this ) )
, mpBufferSize( new QSpinBox( this ) )
, mpConfig( new ConfigDialog( this ) )
, mBufferSize(500)
, mSLARTCom()
{
   QGridLayout *mainLayout   = new QGridLayout( this );

   QStringList applications;
   applications << "Partyman" << "Stripped" << "Funkytown" << "Rubberbandman" << "Karmadrome";
   
   mpBufferSizeLabel->setAlignment( Qt::AlignRight | Qt::AlignVCenter );
   mpBufferSize->setRange( 50, 50000 );
   
   QLabel *mpLogo = new QLabel( this );
   mpLogo->setText( QApplication::applicationName() );
   mpLogo->setAlignment( Qt::AlignCenter );
   mpLogo->setFrameShadow( QFrame::Raised );
   mpLogo->setFrameShape( QFrame::Box );

   mainLayout->addWidget( mpLogo,            0, 0, 1, applications.count() );
   mainLayout->addWidget( mpMessageBuffer,   1, 0, 1, applications.count() );
   for( int i = 0; i < applications.count(); i++ )
   {
      mainLayout->addWidget( new ExecButton( applications.at(i), this ), 2, i );
   }
   mainLayout->addWidget( mpSettingsButton,  3, 0 );
   mainLayout->addWidget( mpBufferSizeLabel, 3, applications.count() - 2 );
   mainLayout->addWidget( mpBufferSize,      3, applications.count() - 1 );

   connect( mpSettingsButton, SIGNAL(clicked()),
            this, SLOT(handleSettings()) );
   connect( mpBufferSize, SIGNAL(valueChanged(int)),
            this, SLOT(setBufferSize(int)) );
   connect( &mSLARTCom, SIGNAL(packageRead(QStringList)),
            this, SLOT(handleSLART(QStringList)) );
   connect( &mSLARTCom, SIGNAL(updateConfig()),
            mpConfig, SLOT(readSettings()) );
   connect( mpMessageBuffer, SIGNAL(itemClicked(QListWidgetItem*)),
            this, SLOT(listWidgetItemToClipboard(QListWidgetItem*)) );

   readConfig();
   
   setLayout( mainLayout );
}


void MainWidget::readConfig()
{
   MySettings settings;

   mSLARTCom.resetReceiver();

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


void MainWidget::handleSLART( const QStringList &message )
{
   mpMessageBuffer->addItem( "> "+QDateTime::currentDateTime().toString() );

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


void MainWidget::listWidgetItemToClipboard( QListWidgetItem *item )
{
   MyClipboard::set( item->text() );
}

