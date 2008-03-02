/**
 * ProxyWidget.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#include "ProxyWidget.hpp"
#include "MySettings.hpp"

#include <QtGui>
#include <QHttp>


ProxyWidget::ProxyWidget( QWidget *parent )
: QGroupBox( parent )
, mpProxyOnLabel( new QLabel( tr("Enable Proxy"), this ) )
, mpProxyHostLabel( new QLabel( tr(" Host"), this ) )
, mpProxyPortLabel( new QLabel( tr(" Port"), this ) )
, mpProxyAuthLabel( new QLabel( tr("Authenication"), this ) )
, mpProxyLoginLabel( new QLabel( tr("Login"), this ) )
, mpProxyPasswordLabel( new QLabel( tr("Password"), this ) )
, mpProxyOnButton( new QPushButton )
, mpProxyHostInput( new QLineEdit )
, mpProxyPortInput( new QSpinBox )
, mpProxyAuthButton( new QPushButton )
, mpProxyLoginInput( new QLineEdit )
, mpProxyPasswordInput( new QLineEdit )
{
   QGridLayout *mainLayout = new QGridLayout( this );
   setTitle( tr("Proxy Settings:") );
   
   mpProxyOnButton->setCheckable( true );
   mpProxyAuthButton->setCheckable( true );
   mpProxyPortInput->setRange( 1, 65535 );
   mpProxyPasswordInput->setEchoMode( QLineEdit::PasswordEchoOnEdit );

   readSettings();
   
   mainLayout->addWidget( mpProxyOnLabel,        0, 0, 1, 1 );
   mainLayout->addWidget( mpProxyHostLabel,      1, 0, 1, 1 );
   mainLayout->addWidget( mpProxyPortLabel,      2, 0, 1, 1 );
   mainLayout->addWidget( mpProxyAuthLabel,      0, 2, 1, 1 );
   mainLayout->addWidget( mpProxyLoginLabel,     1, 2, 1, 1 );
   mainLayout->addWidget( mpProxyPasswordLabel,  2, 2, 1, 1 );

   mainLayout->addWidget( mpProxyOnButton,      0, 1, 1, 1 );
   mainLayout->addWidget( mpProxyHostInput,     1, 1, 1, 1 );
   mainLayout->addWidget( mpProxyPortInput,     2, 1, 1, 1 );
   mainLayout->addWidget( mpProxyAuthButton,    0, 3, 1, 1 );
   mainLayout->addWidget( mpProxyLoginInput,    1, 3, 1, 1 );
   mainLayout->addWidget( mpProxyPasswordInput, 2, 3, 1, 1 );
 
   setLayout( mainLayout );
   
   connect( mpProxyOnButton,   SIGNAL(clicked()), this, SLOT(updateWidgets()) );
   connect( mpProxyAuthButton, SIGNAL(clicked()), this, SLOT(updateWidgets()) );
}


ProxyWidget::~ProxyWidget()
{
}


void ProxyWidget::updateWidgets( bool disabled )
{
   if( disabled )
   {
      mpProxyOnLabel->setDisabled( true );
      mpProxyOnButton->setDisabled( true );
      mpProxyHostLabel->setDisabled( true );
      mpProxyHostInput->setDisabled( true );
      mpProxyPortLabel->setDisabled( true );
      mpProxyPortInput->setDisabled( true );
      mpProxyAuthLabel->setDisabled( true );
      mpProxyAuthButton->setDisabled( true );
      mpProxyLoginLabel->setDisabled( true );
      mpProxyLoginInput->setDisabled( true );
      mpProxyPasswordLabel->setDisabled( true );
      mpProxyPasswordInput->setDisabled( true );
   }
   else
   {
      mpProxyOnLabel->setDisabled( false );
      mpProxyOnButton->setDisabled( false );
      if( mpProxyOnButton->isChecked() )
      {
         mpProxyOnButton->setText( tr("on") );
      }
      else
      {
         mpProxyOnButton->setText( tr("off") );
      }
      
      if( mpProxyAuthButton->isChecked() )
      {
         mpProxyAuthButton->setText( tr("on") );
      }
      else
      {
         mpProxyAuthButton->setText( tr("off") );
      }
      
      if( mpProxyOnButton->isChecked() )
      {
         mpProxyHostLabel->setDisabled( false );
         mpProxyHostInput->setDisabled( false );
         mpProxyPortLabel->setDisabled( false );
         mpProxyPortInput->setDisabled( false );
         mpProxyAuthButton->setDisabled( false );
         mpProxyAuthLabel->setDisabled( false );
         if( mpProxyAuthButton->isChecked() )
         {
            mpProxyLoginInput->setDisabled( false );
            mpProxyLoginLabel->setDisabled( false );
            mpProxyPasswordInput->setDisabled( false );
            mpProxyPasswordLabel->setDisabled( false );
         }
         else
         {
            mpProxyLoginLabel->setDisabled( true );
            mpProxyLoginInput->setDisabled( true );
            mpProxyPasswordLabel->setDisabled( true );
            mpProxyPasswordInput->setDisabled( true );
         }
      }
      else
      {
         mpProxyHostLabel->setDisabled( true );
         mpProxyHostInput->setDisabled( true );
         mpProxyPortLabel->setDisabled( true );
         mpProxyPortInput->setDisabled( true );
         mpProxyAuthLabel->setDisabled( true );
         mpProxyAuthButton->setDisabled( true );
         mpProxyLoginLabel->setDisabled( true );
         mpProxyLoginInput->setDisabled( true );
         mpProxyPasswordLabel->setDisabled( true );
         mpProxyPasswordInput->setDisabled( true );
      }
   }
}


void ProxyWidget::setProxy( QHttp *http )
{
   QString host;
   int     port = 0;
   QString login;
   QString password;
   
   if( mpProxyOnButton->isChecked() )
   {
      host = mpProxyHostInput->text();
      port = mpProxyPortInput->value();
      if( mpProxyAuthButton->isChecked() )
      {
         login    = mpProxyLoginInput->text();
         password = mpProxyPasswordInput->text();
      }
   }
   http->setProxy( host, port, login, password );
}


void ProxyWidget::readSettings()
{
   QSettings settings( QApplication::organizationName(), "Global" );

   settings.beginGroup( "HTTPProxy" );
   mpProxyOnButton->setChecked(   settings.value("Enable",   false).toBool() );
   mpProxyHostInput->setText(     settings.value("Host",     QString( tr("proxy") )).toString() );
   mpProxyPortInput->setValue(    settings.value("Port",     8080).toInt() );
   mpProxyAuthButton->setChecked( settings.value("Auth",     false).toBool() );
   mpProxyLoginInput->setText(    settings.value("Login",    QString( tr("login") )).toString() );
   mpProxyPasswordInput->setText( settings.value("Password", QString( tr("password") )).toString() );
   
   updateWidgets();
}


void ProxyWidget::writeSettings()
{
   QSettings settings( QApplication::organizationName(), "Global" );
   
   settings.beginGroup( "HTTPProxy" );
   settings.setValue( "Enable",   mpProxyOnButton->isChecked() );
   settings.setValue( "Host",     mpProxyHostInput->text() );
   settings.setValue( "Port",     mpProxyPortInput->value() );
   settings.setValue( "Auth",     mpProxyAuthButton->isChecked() );
   settings.setValue( "Login",    mpProxyLoginInput->text() );
   settings.setValue( "Password", mpProxyPasswordInput->text() );
   settings.sync();
   
   MySettings().sendUdpMessage( "CFG\n" );
}
