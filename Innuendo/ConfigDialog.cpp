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
#include "Version.hpp"


#include <QtGui>
#include "SLARTCom.hpp"


ConfigDialog::ConfigDialog( QWidget *parent, Qt::WindowFlags flags )
: QDialog( parent, flags )
, mpNotifyWidget( new ConfigNotifyWidget( this ) )
, mpGlobalConfigWidget( new GlobalConfigWidget( this ) )
, mpProxyWidget( new ProxyWidget( this ) )
{
   setWindowTitle( QApplication::applicationName()+tr(" Settings") );
   
   QLabel *aboutText = new QLabel( tr("Innuendo Version " SLART_VERSION
   " written by Sven Oliver Moll as a part of <a href='http://svolli.org/software/slart'>SLART</a>.<br>"
   "Distributed unter the terms of the <a href='http://www.gnu.org/licenses/gpl.html'>GPL</a>."), this );
   aboutText->setOpenExternalLinks( true );
   mpGlobalConfigWidget->showClipboard();
   
   readSettings();
   
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
      tmp2Layout->addWidget( aboutText );
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
      mainLayout->addWidget( aboutText );
      mainLayout->addLayout( buttonLayout );
   }
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

