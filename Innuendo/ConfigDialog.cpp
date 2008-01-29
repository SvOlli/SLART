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


ConfigDialog::ConfigDialog( QWidget *parent, Qt::WindowFlags flags )
: QDialog( parent, flags )
, mpProxyWidget( new ProxyWidget( this ) )
, mpSLATCommunication( new QCheckBox( tr("Use SLAT UDP Communication"), this ) )
, mpUDPListenerPort( new QSpinBox( this ) )
{
   setWindowTitle( QApplication::applicationName()+tr(" Settings") );
   
   connect( mpSLATCommunication, SIGNAL(clicked(bool)),
            this, SLOT(handleUDPListen(bool)) );
   
   mpUDPListenerPort->setRange( 1, 65535 );
   readSettings();
   
   QGroupBox   *pmGroup  = new QGroupBox( tr("Partyman Parameters:"), this );
   QGridLayout *pmLayout = new QGridLayout( pmGroup );
   pmLayout->addWidget( mpSLATCommunication, 2, 0 );
   pmLayout->addWidget( mpUDPListenerPort, 2, 1 );
   
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
   
   emit configChanged();
}


void ConfigDialog::writeSettings()
{
   MySettings settings;

   settings.setValue( "SLATCommunication", mpSLATCommunication->isChecked() );
   settings.setValue( "UDPListenerPort", mpUDPListenerPort->value() );

   emit configChanged();
}


void ConfigDialog::handleUDPListen( bool checked )
{
   mpUDPListenerPort->setDisabled( !checked );
}
