/**
 * ConfigNotifyWidget.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#include "ConfigNotifyWidget.hpp"
#include "ConfigNotifyApplicationWidget.hpp"
#include "Trace.hpp"

#include <QtGui>
#include <QHttp>


ConfigNotifyWidget::ConfigNotifyWidget( QWidget *parent )
: QWidget( parent )
, mpSignalMapper( new QSignalMapper( this ) )
, mpApps( 0 )
, mpTabs( 0 )
, mApplications()
{
   int i;
   
   connect( mpSignalMapper, SIGNAL(mapped(int)),
            this, SLOT(handleAppButton(int)) );
   
   QVBoxLayout *mainLayout   = new QVBoxLayout( this );
   QHBoxLayout *bothLayout   = new QHBoxLayout();
   QVBoxLayout *appsLayout   = new QVBoxLayout();
   QVBoxLayout *tabsLayout   = new QVBoxLayout();
   QHBoxLayout *buttonLayout = new QHBoxLayout;

   mApplications << "Innuendo" << "Partyman" << "Stripped" << "Funkytown" << "Rubberbandman" << "Karmadrome";
   mpApps = new QPushButton*[mApplications.count()];
   mpTabs = new ConfigNotifyApplicationWidget*[mApplications.count()];
   for( i = 0; i < mApplications.count(); i++ )
   {
      mpApps[i] = new QPushButton( mApplications.at(i), this );
      mpTabs[i] = new ConfigNotifyApplicationWidget( i, mApplications, this );
      mpApps[i]->setCheckable( true );
      mpApps[i]->setChecked( i == 0 );
      mpTabs[i]->setVisible( i == 0 );
      appsLayout->addWidget( mpApps[i] );
      tabsLayout->addWidget( mpTabs[i] );
      connect( mpApps[i], SIGNAL(clicked()), mpSignalMapper, SLOT(map()) );
      mpSignalMapper->setMapping( mpApps[i], i );
   }
   
#if QT_VERSION < 0x040300
   mainLayout->setMargin( 1 );
   buttonLayout->setMargin( 0 );
#else
   mainLayout->setContentsMargins( 1, 1, 1, 1 );
   buttonLayout->setContentsMargins( 0, 0, 0, 0 );
#endif
   
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
   
   bothLayout->addLayout( appsLayout );
   bothLayout->addLayout( tabsLayout );
   bothLayout->setStretchFactor( appsLayout, 0 );
   bothLayout->setStretchFactor( tabsLayout, 1 );
   mainLayout->addLayout( bothLayout );
   mainLayout->addLayout( buttonLayout );
   
   connect( allOnButton,  SIGNAL(clicked()),
            this, SLOT(enableFullCommunication()) );
   connect( allOffButton, SIGNAL(clicked()),
            this, SLOT(disableFullCommunication()) );

   setLayout( mainLayout );
}


void ConfigNotifyWidget::handleAppButton( int index )
{
   for( int i = 0; i < mApplications.count(); i++ )
   {
      mpApps[i]->setChecked( i == index );
      mpTabs[i]->setVisible( i == index );
   }
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
