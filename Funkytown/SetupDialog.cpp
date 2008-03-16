/**
 * SetupDialog.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#include "SetupDialog.hpp"
#include "MySettings.hpp"
#include "ProxyWidget.hpp"
#include "Version.hpp"

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
   
   QLabel *aboutText = new QLabel( tr("Funkytown Version " SLART_VERSION
   " written by Sven Oliver Moll as a part of <a href='http://svolli.org/software/slart'>SLART</a>.<br>"
   "Distributed unter the terms of the <a href='http://www.gnu.org/licenses/gpl.html'>GPL</a>."), this );
   aboutText->setOpenExternalLinks( true );
   mpLayout->addWidget( mpProxyWidget,   0, 0, 1, 2 );
   mpLayout->addWidget( mpLogList,       1, 0, 1, 2 );
   mpLayout->addWidget( aboutText,       2, 0, 1, 2 );
   mpLayout->addWidget( mpAboutQtButton, 3, 0, 1, 1 );
   mpLayout->addWidget( mpCloseButton,   3, 1, 1, 1 );
   
   setLayout( mpLayout );
   
   connect( mpCloseButton, SIGNAL(clicked()), this, SLOT(reject()) );
   connect( mpAboutQtButton, SIGNAL(clicked()), qApp, SLOT(aboutQt()) );
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

