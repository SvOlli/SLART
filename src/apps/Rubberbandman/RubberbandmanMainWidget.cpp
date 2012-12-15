/*
 * src/apps/Rubberbandman/RubberbandmanMainWidget.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "RubberbandmanMainWidget.hpp"

/* system headers */

/* Qt headers */
#include <QtGui>

/* local library headers */
#include <DatabaseInterface.hpp>
#include <LEDIcon.hpp>
#include <MySettings.hpp>
#include <Satellite.hpp>
#include <WidgetShot.hpp>

/* local headers */
#include "BrowseWidget.hpp"
#include "ConfigDialog.hpp"
#include "DatabaseWidget.hpp"
#include "SatelliteWidget.hpp"


RubberbandmanMainWidget::RubberbandmanMainWidget( QWidget *parent, Qt::WindowFlags flags )
: QWidget( parent, flags )
, mpBrowseWidget( new BrowseWidget( this ) )
, mpSatelliteWidget( new SatelliteWidget( this ) )
, mpDatabaseWidget( new DatabaseWidget( this ) )
, mpTabs( new QTabWidget( this ) )
, mpSettingsButton( new QPushButton( tr("Settings"), this ) )
, mpDatabaseActivity( new QLabel( this ) )
, mpConfigDialog( new ConfigDialog( this ) )
, mActiveLED( LEDIcon::pixmap( QColor("#ff0000"), 25 ) )
, mIdleLED( LEDIcon::pixmap( QColor("#5f0000"), 25 ) )
{
   MySettings settings;
   QVBoxLayout *mainLayout = new QVBoxLayout( this );
   mainLayout->setContentsMargins( 3, 3, 3, 3 );
   parent->setWindowIcon( QIcon( ":/SLART.png" ) );

   mpTabs->addTab( mpBrowseWidget,    tr("Filesystem") );
   mpTabs->addTab( mpSatelliteWidget, tr("Satellite") );
   mpTabs->addTab( mpDatabaseWidget,  tr("Database") );
   mpTabs->setCurrentIndex( settings.VALUE_CURRENTTAB );

   mainLayout->addWidget( mpTabs );
   QHBoxLayout *bottomLayout( new QHBoxLayout() );
   bottomLayout->addWidget( mpSettingsButton, 1 );
   bottomLayout->addWidget( mpDatabaseActivity, 0 );
   mainLayout->addLayout( bottomLayout );

   connect( mpSatelliteWidget, SIGNAL(showInFilesystem(QString)),
            mpBrowseWidget, SLOT(scrollTo(QString)) );
   connect( mpSatelliteWidget, SIGNAL(showInFilesystem(QString)),
            this, SLOT(goToFilesystem()) );
   connect( mpTabs, SIGNAL(currentChanged(int)),
            this, SLOT(handleTabChange(int)) );
   connect( mpSettingsButton, SIGNAL(clicked()),
            mpConfigDialog, SLOT(exec()) );
   connect( mpSatelliteWidget, SIGNAL(partymanConfigUpdate()),
            mpDatabaseWidget, SLOT(readPartymanConfig()) );
   DatabaseInterface::get()->connectActivityIndicator( this, SLOT(databaseActive(bool)) );

   setLayout( mainLayout );

   mpSettingsButton->setObjectName( QString("SettingsButton") );

   WidgetShot::addWidget( "MainWidget", this );
}


RubberbandmanMainWidget::~RubberbandmanMainWidget()
{
}


void RubberbandmanMainWidget::goToFilesystem()
{
   mpTabs->setCurrentWidget( mpBrowseWidget );
}


void RubberbandmanMainWidget::handleTabChange( int tabNr )
{
   MySettings().setValue( "CurrentTab", tabNr );
}


void RubberbandmanMainWidget::databaseActive( bool on )
{
   mpDatabaseActivity->setPixmap( on ? mActiveLED : mIdleLED );
}
