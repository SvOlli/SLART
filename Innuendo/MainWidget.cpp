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
#include "GlobalConfigWidget.hpp"
#include "ExecButton.hpp"

#include "Trace.hpp"


MainWidget::MainWidget( QWidget *parent, Qt::WindowFlags flags )
: QWidget( parent, flags )
, mpMessageBuffer( new QListWidget( this ) )
, mpSettingsButton( new QPushButton( tr("Settings"), this ) )
, mpPingButton( new QPushButton( tr("Ping"), this ) )
, mpBufferSizeLabel( new QLabel( tr("Buffer Size:"), this ) )
, mpBufferSize( new QSpinBox( this ) )
, mpConfig( new ConfigDialog( this ) )
, mBufferSize(500)
, mSLARTCom()
, mApplications()
{
   QGridLayout *mainLayout   = new QGridLayout( this );
#if QT_VERSION < 0x040300
   mainLayout->setMargin( 3 );
#else
   mainLayout->setContentsMargins( 3, 3, 3, 3 );
#endif

   mApplications << "Partyman" << "Stripped" << "Funkytown" << "Rubberbandman" << "Karmadrome";
   
   mpBufferSizeLabel->setAlignment( Qt::AlignRight | Qt::AlignVCenter );
   mpBufferSize->setRange( 50, 50000 );
   
   QLabel *mpLogo = new QLabel( this );
   mpLogo->setText( QApplication::applicationName() );
   mpLogo->setAlignment( Qt::AlignCenter );
   mpLogo->setFrameShadow( QFrame::Raised );
   mpLogo->setFrameShape( QFrame::Box );

   mainLayout->addWidget( mpLogo,            0, 0, 1, mApplications.count() );
   mainLayout->addWidget( mpMessageBuffer,   1, 0, 1, mApplications.count() );
   for( int i = 0; i < mApplications.count(); i++ )
   {
      mainLayout->addWidget( new ExecButton( mApplications.at(i), this ), 2, i );
   }
   mainLayout->addWidget( mpSettingsButton,  3, 0 );
   mainLayout->addWidget( mpPingButton,      3, 1 );
   mainLayout->addWidget( mpBufferSizeLabel, 3, mApplications.count() - 2 );
   mainLayout->addWidget( mpBufferSize,      3, mApplications.count() - 1 );

   connect( mpSettingsButton, SIGNAL(clicked()),
            mpConfig, SLOT(exec()) );
   connect( mpPingButton, SIGNAL(clicked()),
            this, SLOT(handlePingButton()) );
   connect( mpConfig, SIGNAL(configChanged()),
            this, SLOT(readConfig()) );
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


void MainWidget::handlePingButton()
{
   MySettings settings;
   mpMessageBuffer->addItem( "** pinging applications" );
   mpPingButton->setDisabled( true );
   for( int i = 0; i < mApplications.count(); i++ )
   {
      QString msg("* ");
      msg.append( mApplications.at(i) );
      msg.append( " is " );
      if( !mSLARTCom.ping( mApplications.at(i) ) )
      {
         msg.append( "not " );
      }
      msg.append( "responing" );
      mpMessageBuffer->addItem( msg );
   }
   mpPingButton->setDisabled( false );
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
   GlobalConfigWidget::setClipboard( item->text() );
}

