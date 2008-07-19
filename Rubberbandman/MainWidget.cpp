/**
 * MainWidget.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#include <QtGui>
#include "MainWidget.hpp"

#include "Database.hpp"
#include "BrowseWidget.hpp"
#include "SLARTComWidget.hpp"
#include "DatabaseWidget.hpp"
#include "ConfigDialog.hpp"
#include "MySettings.hpp"


MainWidget::MainWidget( QWidget *parent, Qt::WindowFlags flags )
: QWidget( parent, flags )
, mpDatabase( new Database() )
, mpBrowseWidget( new BrowseWidget( mpDatabase, this ) )
, mpSLARTComWidget( new SLARTComWidget( mpDatabase, this ) )
, mpDatabaseWidget( new DatabaseWidget( mpDatabase, this ) )
, mpTabs( new QTabWidget( this ) )
, mpSettingsButton( new QPushButton( tr("Settings"), this ) )
, mpConfigDialog( new ConfigDialog( this ) )
{
   MySettings settings;
   QVBoxLayout *mainLayout = new QVBoxLayout( this );
#if QT_VERSION < 0x040300
   mainLayout->setMargin( 3 );
#else
   mainLayout->setContentsMargins( 3, 3, 3, 3 );
#endif
   
   mpTabs->addTab( mpBrowseWidget,   tr("Filesystem") );
   mpTabs->addTab( mpSLARTComWidget, tr("Partyman") );
   mpTabs->addTab( mpDatabaseWidget, tr("Database") );
   mpTabs->setCurrentIndex( settings.value("CurrentTab", 0).toInt() );
   
   QLabel *mpLogo = new QLabel( this );
   mpLogo->setText( QApplication::applicationName() );
   mpLogo->setAlignment( Qt::AlignCenter );
   mpLogo->setFrameShadow( QFrame::Raised );
   mpLogo->setFrameShape( QFrame::Box );

   mainLayout->addWidget( mpLogo );
   mainLayout->addWidget( mpTabs );
   mainLayout->addWidget( mpSettingsButton );
   
   connect( mpSLARTComWidget, SIGNAL( showInFilesystem(const QString&) ),
            mpBrowseWidget, SLOT(scrollTo(const QString&)) );
   connect( mpSLARTComWidget, SIGNAL( showInFilesystem(const QString&) ),
            this, SLOT( goToFilesystem() ) );
   connect( mpTabs, SIGNAL(currentChanged(int)),
            this, SLOT(handleTabChange(int)) );
   connect( mpSettingsButton, SIGNAL(clicked()),
            mpConfigDialog, SLOT(exec()) );
   
   setLayout( mainLayout );
}


MainWidget::~MainWidget()
{
   delete mpDatabase;
}


void MainWidget::goToFilesystem()
{
   mpTabs->setCurrentWidget( mpBrowseWidget );
}


void MainWidget::handleTabChange( int tabNr )
{
   MySettings().setValue( "CurrentTab", tabNr );
}
