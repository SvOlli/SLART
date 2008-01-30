/**
 * ConfigDialog.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 */

#include "ConfigDialog.hpp"
#include "MySettings.hpp"
#include "ProxyWidget.hpp"

#include <QtGui>
#include "SLATCom.hpp"


ConfigDialog::ConfigDialog( QWidget *parent, Qt::WindowFlags flags )
: QDialog( parent, flags )
, mpProxyWidget( new ProxyWidget( this ) )
, mpSLATCommunication( new QCheckBox( tr("Use SLAT UDP Communication"), this ) )
, mpUDPListenerPort( new QSpinBox( this ) )
, mpPartymanNotify( new QCheckBox( tr("Partyman notification requested"), this ) )
, mpStrippedNotify( new QCheckBox( tr("Stripped notification requested"), this ) )
, mpRubberbandmanNotify( new QCheckBox( tr("Rubberbandman notification requested"), this ) )
, mpFunkytownNotify( new QCheckBox( tr("Funkytown notification requested"), this ) )
{
   setWindowTitle( QApplication::applicationName()+tr(" Settings") );
   
   connect( mpSLATCommunication, SIGNAL(clicked(bool)),
            this, SLOT(handleUDPListen(bool)) );
   
   mpUDPListenerPort->setRange( 1, 65535 );
   readSettings();
   
   QGroupBox   *pmGroup  = new QGroupBox( QApplication::applicationName() + tr(" Parameters:"), this );
   QGridLayout *pmLayout = new QGridLayout( pmGroup );
   pmLayout->addWidget( mpSLATCommunication,   2, 0 );
   pmLayout->addWidget( mpUDPListenerPort,     2, 1 );
   pmLayout->addWidget( mpPartymanNotify,      3, 0, 1, 2 );
   pmLayout->addWidget( mpStrippedNotify,      4, 0, 1, 2 );
   pmLayout->addWidget( mpRubberbandmanNotify, 5, 0, 1, 2 );
   pmLayout->addWidget( mpFunkytownNotify,     6, 0, 1, 2 );
   
   QPushButton *okButton     = new QPushButton( tr("OK"), this );
   QPushButton *cancelButton = new QPushButton( tr("Cancel"), this );

   QHBoxLayout *buttonLayout = new QHBoxLayout;
   buttonLayout->addWidget( okButton );
   buttonLayout->addWidget( cancelButton );

   QVBoxLayout *mainLayout = new QVBoxLayout;
   
   if( QApplication::desktop()->screenGeometry().height() < 600 )
   {
      QHBoxLayout *interimLayout = new QHBoxLayout;
      interimLayout->addWidget( mpProxyWidget );
      interimLayout->addWidget( pmGroup );
      mainLayout->addLayout( interimLayout );
   }
   else
   {
      mainLayout->addWidget( mpProxyWidget );
      mainLayout->addWidget( pmGroup );
   }
   mainLayout->addLayout( buttonLayout );
   setLayout( mainLayout );

   connect( okButton,      SIGNAL(clicked()), this, SLOT(accept()) );
   connect( cancelButton,  SIGNAL(clicked()), this, SLOT(reject()) );
   connect( this,         SIGNAL(accepted()), this, SLOT(writeSettings()) );
}


void ConfigDialog::readSettings()
{
   MySettings settings;
   
   mpSLATCommunication->setChecked( settings.value("SLATCommunication", false).toBool() );
   mpUDPListenerPort->setValue(settings.value("UDPListenerPort", 24220).toInt() );
   
   mpPartymanNotify->setChecked( settings.listensOn( "Partyman" ) );
   mpStrippedNotify->setChecked( settings.listensOn( "Stripped" ) );
   mpRubberbandmanNotify->setChecked( settings.listensOn( "Rubberbandman" ) );
   mpFunkytownNotify->setChecked( settings.listensOn( "Funkytown" ) );
   
   emit configChanged();
}


void ConfigDialog::writeSettings()
{
   MySettings settings;

   settings.setValue( "SLATCommunication", mpSLATCommunication->isChecked() );
   settings.setValue( "UDPListenerPort", mpUDPListenerPort->value() );

   settings.requestNotification( "Partyman", mpPartymanNotify->isChecked() );
   settings.requestNotification( "Funkytown", mpFunkytownNotify->isChecked() );
   settings.requestNotification( "Rubberbandman", mpRubberbandmanNotify->isChecked() );
   settings.requestNotification( "Stripped", mpStrippedNotify->isChecked() );

   emit configChanged();
}


void ConfigDialog::handleUDPListen( bool checked )
{
   mpUDPListenerPort->setDisabled( !checked );
}

