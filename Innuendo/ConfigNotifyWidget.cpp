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
: QGroupBox( parent )
//, mpTabBar( new QTabBar( this ) )
, mpTabWidget( new QTabWidget( this ) )
, mpTabs( 0 )
{
   int i;
   mApplications << "Innuendo" << "Partyman" << "Stripped" << "Funkytown" << "Rubberbandman";
   mpTabs = new ConfigNotifyApplicationWidget*[mApplications.count()];
   for( i = 0; i < mApplications.count(); i++ )
   {
      mpTabs[i] = new ConfigNotifyApplicationWidget( i, mApplications, this );
      mpTabWidget->addTab( mpTabs[i], mApplications.at(i) );
   }
   
   MySettings settings;
   QGridLayout *mainLayout = new QGridLayout( this );
   setTitle( tr("Notification Settings:") );
   
//   mainLayout->addWidget( mpTabBar, 0, 0, mApplications.count() + 1, 1 );
   mainLayout->addWidget( mpTabWidget, 0, 0, mApplications.count() + 1, 1 );
   
   mpTabs[mApplications.indexOf("Partyman")]->allowNotify(mApplications.indexOf("Innuendo"));
   mpTabs[mApplications.indexOf("Partyman")]->allowNotify(mApplications.indexOf("Rubberbandman"));
   
   mpTabs[mApplications.indexOf("Stripped")]->allowNotify(mApplications.indexOf("Innuendo"));
   mpTabs[mApplications.indexOf("Stripped")]->allowNotify(mApplications.indexOf("Partyman"));
   
   mpTabs[mApplications.indexOf("Funkytown")]->allowNotify(mApplications.indexOf("Innuendo"));
   mpTabs[mApplications.indexOf("Funkytown")]->allowNotify(mApplications.indexOf("Partyman"));
   
   mpTabs[mApplications.indexOf("Rubberbandman")]->allowNotify(mApplications.indexOf("Innuendo"));
 
   
   setLayout( mainLayout );
}


ConfigNotifyWidget::~ConfigNotifyWidget()
{
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
