/**
 * ConfigDialog.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 */

#include "ConfigDialog.hpp"
#include "MySettings.hpp"
#include "ProxyWidget.hpp"
#include "ConfigNotifyWidget.hpp"
#include "GlobalConfigWidget.hpp"


#include <QtGui>
#include "SLARTCom.hpp"


ConfigDialog::ConfigDialog( QWidget *parent, Qt::WindowFlags flags )
: QDialog( parent, flags )
, mpNotifyWidget( new ConfigNotifyWidget( this ) )
, mpGlobalConfigWidget( new GlobalConfigWidget( this ) )
, mpProxyWidget( new ProxyWidget( this ) )
{
   setWindowTitle( QApplication::applicationName()+tr(" Settings") );
   
   mpGlobalConfigWidget->showClipboard();
   
   readSettings();
   
   QPushButton *okButton     = new QPushButton( tr("OK"), this );
   QPushButton *cancelButton = new QPushButton( tr("Cancel"), this );

   QHBoxLayout *buttonLayout = new QHBoxLayout;
   buttonLayout->addWidget( okButton );
   buttonLayout->addWidget( cancelButton );

   QVBoxLayout *mainLayout = new QVBoxLayout;
   
   if( QApplication::desktop()->screenGeometry().height() < 600 )
   {
      QGridLayout *interimLayout = new QGridLayout;
      interimLayout->addWidget( mpNotifyWidget, 0, 0, 2, 1 );
      interimLayout->addWidget( mpGlobalConfigWidget, 0, 1 );
      interimLayout->addWidget( mpProxyWidget, 1, 1 );
      mainLayout->addLayout( interimLayout );
   }
   else
   {
      mainLayout->addWidget( mpNotifyWidget );
      mainLayout->addWidget( mpGlobalConfigWidget );
      mainLayout->addWidget( mpProxyWidget );
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
   
   mpNotifyWidget->readSettings();
   mpGlobalConfigWidget->readSettings();
   mpProxyWidget->readSettings();
   
   emit configChanged();
}


void ConfigDialog::writeSettings()
{
   MySettings settings;

   mpNotifyWidget->writeSettings();
   mpGlobalConfigWidget->writeSettings();
   mpProxyWidget->writeSettings();

   emit configChanged();
}

