/**
 * SetupDialog.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#include "SetupDialog.hpp"
#include "MySettings.hpp"
#include "ProxyWidget.hpp"

#include <QtGui>


SetupDialog::SetupDialog( QWidget *parent )
: QDialog( parent )
, mpLayout( new QGridLayout )
, mpProxyWidget( new ProxyWidget )
, mpLogList( new QListWidget )
, mpCloseButton( new QPushButton(tr("Close"), this) )
, mpAboutQtButton( new QPushButton(tr("About Qt"), this) )
{
   MySettings settings;
   
   mpLayout->addWidget( mpProxyWidget,   0, 0, 1, 2 );
   mpLayout->addWidget( mpLogList,       1, 0, 1, 2 );
   mpLayout->addWidget( mpAboutQtButton, 2, 0, 1, 1 );
   mpLayout->addWidget( mpCloseButton,   2, 1, 1, 1 );
   
   setLayout( mpLayout );
   
   connect( mpCloseButton,     SIGNAL(clicked()), this, SLOT(reject()) );
   connect( mpAboutQtButton,   SIGNAL(clicked()), qApp, SLOT(aboutQt()) );
}


SetupDialog::~SetupDialog()
{
}


ProxyWidget *SetupDialog::proxyWidget()
{
   return mpProxyWidget;
}


void SetupDialog::logMessage( const QString &message )
{
   mpLogList->addItem( message );
   mpLogList->scrollToBottom();
}

