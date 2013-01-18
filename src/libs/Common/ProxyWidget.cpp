/*
 * src/libs/Common/ProxyWidget.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

/* class declaration */
#include "ProxyWidget.hpp"

/* system headers */

/* Qt headers */
#include <QtGui>
#include <QHttp>
#include <QNetworkAccessManager>
#include <QNetworkProxy>

/* local library headers */

/* local headers */
#include "Settings.hpp"


ProxyWidget::ProxyWidget( QWidget *parent )
: QWidget( parent )
, mpProxyOnLabel( new QLabel( tr("Enable Proxy"), this ) )
, mpProxyHostLabel( new QLabel( tr("Host"), this ) )
, mpProxyPortLabel( new QLabel( tr("Port"), this ) )
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

   mainLayout->setRowStretch( 3, 1 );

   setLayout( mainLayout );

   connect( mpProxyOnButton, SIGNAL(clicked()),
            this, SLOT(updateWidgets()) );
   connect( mpProxyAuthButton, SIGNAL(clicked()),
            this, SLOT(updateWidgets()) );
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
   QString      host;
   int          port = 0;
   QString      login;
   QString      password;

   if( Settings::value( Settings::GlobalHTTPProxyEnable ) )
   {
      host = Settings::value( Settings::GlobalHTTPProxyHost );
      port = Settings::value( Settings::GlobalHTTPProxyPort );
      if( Settings::value( Settings::GlobalHTTPProxyAuth ) )
      {
         login    = Settings::value( Settings::GlobalHTTPProxyLogin );
         password = Settings::value( Settings::GlobalHTTPProxyPassword );
      }
   }
   http->setProxy( host, port, login, password );
}


void ProxyWidget::setProxy( QNetworkAccessManager *nam )
{
   QNetworkProxy::ProxyType   type = QNetworkProxy::NoProxy;
   QString                    host;
   int                        port = 0;
   QString                    login;
   QString                    password;

   if( Settings::value( Settings::GlobalHTTPProxyEnable ) )
   {
      type = QNetworkProxy::HttpCachingProxy;
      host = Settings::value( Settings::GlobalHTTPProxyHost );
      port = Settings::value( Settings::GlobalHTTPProxyPort );
      if( Settings::value( Settings::GlobalHTTPProxyAuth ) )
      {
         login    = Settings::value( Settings::GlobalHTTPProxyLogin );
         password = Settings::value( Settings::GlobalHTTPProxyPassword );
      }
   }
   nam->setProxy( QNetworkProxy( type, host, port, login, password ) );
}


void ProxyWidget::readSettings()
{
   mpProxyOnButton->setChecked(   Settings::value( Settings::GlobalHTTPProxyEnable ) );
   mpProxyHostInput->setText(     Settings::value( Settings::GlobalHTTPProxyHost ) );
   mpProxyPortInput->setValue(    Settings::value( Settings::GlobalHTTPProxyPort ) );
   mpProxyAuthButton->setChecked( Settings::value( Settings::GlobalHTTPProxyAuth ) );
   mpProxyLoginInput->setText(    Settings::value( Settings::GlobalHTTPProxyLogin ) );
   mpProxyPasswordInput->setText( Settings::value( Settings::GlobalHTTPProxyPassword ) );

   updateWidgets();
}


void ProxyWidget::writeSettings()
{
   Settings::setValue( Settings::GlobalHTTPProxyEnable,   mpProxyOnButton->isChecked() );
   Settings::setValue( Settings::GlobalHTTPProxyHost,     mpProxyHostInput->text() );
   Settings::setValue( Settings::GlobalHTTPProxyPort,     mpProxyPortInput->value() );
   Settings::setValue( Settings::GlobalHTTPProxyAuth,     mpProxyAuthButton->isChecked() );
   Settings::setValue( Settings::GlobalHTTPProxyLogin,    mpProxyLoginInput->text() );
   Settings::setValue( Settings::GlobalHTTPProxyPassword, mpProxyPasswordInput->text() );

#if 0
   // \todo: adept to Satellite
   MySettings().sendUdpMessage( "CFG\n" );
#endif
}
