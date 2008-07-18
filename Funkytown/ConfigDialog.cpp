/**
 * ConfigDialog.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#include "ConfigDialog.hpp"
#include "MySettings.hpp"
#include "ProxyWidget.hpp"
#include "AboutWidget.hpp"

#include <QtGui>


ConfigDialog::ConfigDialog( QWidget *parent )
: QDialog( parent )
, mpProxyWidget( new ProxyWidget )
, mpLogList( new QListWidget )
{
   setWindowTitle( QApplication::applicationName()+tr(" Settings") );
   
   AboutWidget *about( new AboutWidget( this ) );
   QPushButton *okButton( new QPushButton(tr("OK"), this) );
   QPushButton *cancelButton( new QPushButton(tr("Cancel"), this) );
   
   QHBoxLayout *buttonLayout = new QHBoxLayout;
   buttonLayout->addWidget( okButton );
   buttonLayout->addWidget( cancelButton );
   
   QBoxLayout *mainLayout = new QVBoxLayout( this );
   QTabWidget *tabs       = new QTabWidget( this );
   tabs->addTab( mpProxyWidget, QString(tr("Proxy")) );
   tabs->addTab( mpLogList,     QString(tr("Log")) );
   
   mainLayout->addWidget( about );
   mainLayout->addWidget( tabs );
   mainLayout->addLayout( buttonLayout );
   
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


ProxyWidget *ConfigDialog::proxyWidget()
{
   return mpProxyWidget;
}


void ConfigDialog::logMessage( const QString &message )
{
   mpLogList->addItem( message );
   mpLogList->scrollToBottom();
}


void ConfigDialog::readSettings()
{
   mpProxyWidget->readSettings();
}


void ConfigDialog::writeSettings()
{
   mpProxyWidget->writeSettings();
}

