/**
 * ConfigNotifyApplicationWidget.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#include "ConfigNotifyApplicationWidget.hpp"
#include "MySettings.hpp"

#include <QtGui>
#include <QHttp>


ConfigNotifyApplicationWidget::ConfigNotifyApplicationWidget( int index, const QStringList &apps, ConfigNotifyWidget *parent )
: QWidget( parent )
, mpNotifyWidget( parent )
, mpSLATCommunication( new QCheckBox( tr("Use SLAT UDP Communication"), this ) )
, mpUDPListenerPortLabel( new QLabel( tr("Use UDP Port"), this ) )
, mpUDPListenerPort( new QSpinBox( this ) )
, mpSendsTo( new QCheckBox*[apps.count()] )
, mIndex( index )
, mApplications( apps )
{
   int i;
   
   QVBoxLayout *mainLayout = new QVBoxLayout( this );
   QHBoxLayout *portLayout = new QHBoxLayout;
   
   connect( mpSLATCommunication, SIGNAL(clicked(bool)),
            this, SLOT(handleUDPListen(bool)) );
   
   mpUDPListenerPort->setRange( 1, 65535 );
   mpUDPListenerPort->setRange( 1, 65535 );
   
   mainLayout->addWidget( mpSLATCommunication );
   portLayout->addWidget( mpUDPListenerPortLabel );
   portLayout->addWidget( mpUDPListenerPort );
   mainLayout->addLayout( portLayout );
   
   for( i = 0; i < apps.count(); i++ )
   {
      mpSendsTo[i] = new QCheckBox( QString(tr("Send Notifications To "))+apps.at(i), this );
      mainLayout->addWidget( mpSendsTo[i] );
      mpSendsTo[i]->setHidden( true );
   }
   mainLayout->addWidget( new QWidget() );
   
   setLayout( mainLayout );
}


ConfigNotifyApplicationWidget::~ConfigNotifyApplicationWidget()
{
   delete[] mpSendsTo;
}


void ConfigNotifyApplicationWidget::allowNotify( int index, bool allow )
{
   mpSendsTo[index]->setHidden( !allow );
}


void ConfigNotifyApplicationWidget::readSettings()
{
   QSettings settings( QApplication::organizationName(), mApplications.at( mIndex ) );
   mpSLATCommunication->setChecked( settings.value( "SLATCommunication", false ).toBool() );
   mpUDPListenerPort->setValue( settings.value( "UDPListenerPort", 24221+mIndex ).toInt() );
   
   settings.beginGroup( "Listeners" );
   for( int i = 0; i < mApplications.count(); i++ )
   {
      mpSendsTo[i]->setChecked( settings.value( mApplications.at(i), 0 ).toInt() != 0 );
   }
   settings.endGroup();
}


void ConfigNotifyApplicationWidget::writeSettings()
{
   QSettings settings( QApplication::organizationName(), mApplications.at( mIndex ) );
   settings.setValue( "SLATCommunication", mpSLATCommunication->isChecked() );
   settings.setValue( "UDPListenerPort",   mpUDPListenerPort->value() );
   
   settings.beginGroup( "Listeners" );
   for( int i = 0; i < mApplications.count(); i++ )
   {
      if( mpSendsTo[i]->isChecked() )
      {
         settings.setValue( mApplications.at(i), mpNotifyWidget->getUDPListenerPort(i) );
      }
      else
      {
         settings.remove( mApplications.at(i) );
      }
   }
   settings.endGroup();
}


void ConfigNotifyApplicationWidget::handleUDPListen( bool checked )
{
   mpUDPListenerPort->setDisabled( !checked );
   mpUDPListenerPortLabel->setDisabled( !checked );
   for( int i = 0; i < mApplications.count(); i++ )
   {
      if( !mpSendsTo[i]->isHidden() )
      {
         mpSendsTo[i]->setDisabled( !checked );
      }
   }
}


int ConfigNotifyApplicationWidget::getUDPListenerPort()
{
   return mpUDPListenerPort->value();
}


void ConfigNotifyApplicationWidget::setAllAtOnce( bool enable )
{
   mpSLATCommunication->setChecked( enable );
   
   for( int i = 0; i < mApplications.count(); i++ )
   {
      if( !mpSendsTo[i]->isHidden() )
      {
         mpSendsTo[i]->setChecked( enable );
      }
   }
   handleUDPListen( enable );
}
