/**
 * ConfigNotifyWidget.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#include "ConfigNotifyWidget.hpp"
#include "ConfigNotifyApplicationWidget.hpp"
#include "MySettings.hpp"

#include <QtGui>
#include <QHttp>


ConfigNotifyWidget::ConfigNotifyWidget( QWidget *parent )
: QWidget( parent )
//, mpTabBar( new QTabBar( this ) )
, mpTabWidget( new QTabWidget( this ) )
, mpTabs( 0 )
{
   int i;
   
   QVBoxLayout *mainLayout   = new QVBoxLayout( this );
   QGroupBox   *groupBox     = new QGroupBox;
   QHBoxLayout *buttonLayout = new QHBoxLayout;

   mpTabWidget->setUsesScrollButtons( true );
   mApplications << "Innuendo" << "Partyman" << "Stripped" << "Funkytown" << "Rubberbandman" << "Karmadrome";
   mpTabs = new ConfigNotifyApplicationWidget*[mApplications.count()];
   for( i = 0; i < mApplications.count(); i++ )
   {
      mpTabs[i] = new ConfigNotifyApplicationWidget( i, mApplications, this );
      mpTabWidget->addTab( mpTabs[i], mApplications.at(i) );
   }
   
   MySettings settings;
   QGridLayout *gridLayout = new QGridLayout;
   groupBox->setTitle( tr("Notification Settings:") );
   
#if QT_VERSION < 0x040300
   gridLayout->setMargin( 1 );
   mainLayout->setMargin( 1 );
   buttonLayout->setMargin( 0 );
#else
   gridLayout->setContentsMargins( 1, 1, 2, 2 );
   mainLayout->setContentsMargins( 1, 1, 1, 1 );
   buttonLayout->setContentsMargins( 0, 0, 0, 0 );
#endif
   
//   mainLayout->addWidget( mpTabBar, 0, 0, mApplications.count() + 1, 1 );
   gridLayout->addWidget( mpTabWidget, 0, 0, mApplications.count() + 1, 1 );
   
   /* all communication has been disabled by default,
      now turn those back on which could be useful */
   mpTabs[mApplications.indexOf("Partyman")]->allowNotify(mApplications.indexOf("Innuendo"));
   mpTabs[mApplications.indexOf("Partyman")]->allowNotify(mApplications.indexOf("Rubberbandman"));
   mpTabs[mApplications.indexOf("Partyman")]->allowNotify(mApplications.indexOf("Karmadrome"));
   
   mpTabs[mApplications.indexOf("Stripped")]->allowNotify(mApplications.indexOf("Innuendo"));
   mpTabs[mApplications.indexOf("Stripped")]->allowNotify(mApplications.indexOf("Partyman"));
   
   mpTabs[mApplications.indexOf("Funkytown")]->allowNotify(mApplications.indexOf("Innuendo"));
   mpTabs[mApplications.indexOf("Funkytown")]->allowNotify(mApplications.indexOf("Partyman"));
   
   mpTabs[mApplications.indexOf("Rubberbandman")]->allowNotify(mApplications.indexOf("Innuendo"));
   
   mpTabs[mApplications.indexOf("Karmadrome")]->allowNotify(mApplications.indexOf("Innuendo"));
   
   QPushButton *allOnButton  = new QPushButton( tr("Full Communication"), this );
   QPushButton *allOffButton = new QPushButton( tr("No Communication"), this );
   buttonLayout->addWidget( allOnButton );
   buttonLayout->addWidget( allOffButton );
   
   groupBox->setLayout( gridLayout );
   
   mainLayout->addWidget( groupBox );
   mainLayout->addLayout( buttonLayout );
   
   connect( allOnButton,  SIGNAL(clicked()), this, SLOT(enableFullCommunication()) );
   connect( allOffButton, SIGNAL(clicked()), this, SLOT(disableFullCommunication()) );
   
   setLayout( mainLayout );
}


void ConfigNotifyWidget::readSettings()
{
   for( int i = 0; i < mApplications.count(); i++ )
   {
      mpTabs[i]->readSettings();
   }
}


void ConfigNotifyWidget::writeSettings()
{
   for( int i = 0; i < mApplications.count(); i++ )
   {
      mpTabs[i]->writeSettings();
   }
}


int ConfigNotifyWidget::getUDPListenerPort( int index )
{
   if( (index >= 0) && (index < mApplications.count() ) )
   {
      return mpTabs[index]->getUDPListenerPort();
   }
   else
   {
      return 0;
   }
}


void ConfigNotifyWidget::enableFullCommunication()
{
   setAllAtOnce( true );
}


void ConfigNotifyWidget::disableFullCommunication()
{
   setAllAtOnce( false );
}


void ConfigNotifyWidget::setAllAtOnce( bool enable )
{
   for( int i = 0; i < mApplications.count(); i++ )
   {
      mpTabs[i]->setAllAtOnce( enable );
   }
}
