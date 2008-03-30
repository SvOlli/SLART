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
, mpLayout( new QGridLayout )
, mpProxyWidget( new ProxyWidget )
, mpLogList( new QListWidget )
{
   AboutWidget *about( new AboutWidget( this ) );
   QPushButton *okButton( new QPushButton(tr("OK"), this) );
   QPushButton *cancelButton( new QPushButton(tr("Cancel"), this) );
   
   mpLayout->addWidget( mpProxyWidget,   0, 0, 1, 2 );
   mpLayout->addWidget( mpLogList,       1, 0, 1, 2 );
   mpLayout->addWidget( about,           2, 0, 1, 2 );
   mpLayout->addWidget( okButton,        3, 0, 1, 1 );
   mpLayout->addWidget( cancelButton,    3, 1, 1, 1 );
   
   setLayout( mpLayout );
   
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
