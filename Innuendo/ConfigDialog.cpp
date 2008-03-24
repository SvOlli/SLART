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
#include "AboutWidget.hpp"


#include <QtGui>
#include "SLARTCom.hpp"


ConfigDialog::ConfigDialog( QWidget *parent, Qt::WindowFlags flags )
: QDialog( parent, flags )
, mpNotifyWidget( new ConfigNotifyWidget( this ) )
, mpGlobalConfigWidget( new GlobalConfigWidget( this ) )
, mpProxyWidget( new ProxyWidget( this ) )
{
   setWindowTitle( QApplication::applicationName()+tr(" Settings") );
   
   AboutWidget *about = new AboutWidget( this );
   mpGlobalConfigWidget->showClipboard();
   
   QPushButton *okButton     = new QPushButton( tr("OK"), this );
   QPushButton *cancelButton = new QPushButton( tr("Cancel"), this );

   QHBoxLayout *buttonLayout = new QHBoxLayout;
   buttonLayout->addWidget( okButton );
   buttonLayout->addWidget( cancelButton );

   QBoxLayout *mainLayout;
   
   if( QApplication::desktop()->screenGeometry().height() < 600 )
   {
      mainLayout = new QHBoxLayout( this );
      QVBoxLayout *tmp1Layout = new QVBoxLayout;
      QVBoxLayout *tmp2Layout = new QVBoxLayout;
      tmp1Layout->addWidget( mpNotifyWidget );
      tmp1Layout->addStretch();
      tmp2Layout->addWidget( mpGlobalConfigWidget );
      tmp2Layout->addWidget( mpProxyWidget );
      tmp2Layout->addStretch();
      tmp2Layout->addWidget( about );
      tmp2Layout->addLayout( buttonLayout );
      mainLayout->addLayout( tmp1Layout );
      mainLayout->addLayout( tmp2Layout );
   }
   else
   {
      mainLayout = new QVBoxLayout( this );
      mainLayout->addWidget( mpNotifyWidget );
      mainLayout->addWidget( mpGlobalConfigWidget );
      mainLayout->addWidget( mpProxyWidget );
      mainLayout->addWidget( about );
      mainLayout->addLayout( buttonLayout );
   }
   setLayout( mainLayout );

   connect( okButton, SIGNAL(clicked()),
            this, SLOT(accept()) );
   connect( cancelButton, SIGNAL(clicked()),
            this, SLOT(reject()) );
   connect( this, SIGNAL(accepted()),
            this, SLOT(writeSettings()) );
   connect( this, SIGNAL(rejected()),
            this, SLOT(readSettings()) );
   
   readSettings();
}


void ConfigDialog::exec()
{
   readSettings();
   QDialog::exec();
}


void ConfigDialog::readSettings()
{
   mpNotifyWidget->readSettings();
   mpGlobalConfigWidget->readSettings();
   mpProxyWidget->readSettings();
   
   emit configChanged();
}


void ConfigDialog::writeSettings()
{
   mpNotifyWidget->writeSettings();
   mpGlobalConfigWidget->writeSettings();
   mpProxyWidget->writeSettings();

   emit configChanged();
}

