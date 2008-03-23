/**
 * MainWidget.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#include <QtGui>
#include "MainWidget.hpp"

#include "BrowseWidget.hpp"
#include "SLARTComWidget.hpp"


MainWidget::MainWidget( QWidget *parent, Qt::WindowFlags flags )
: QWidget( parent, flags )
, mpBrowseWidget( new BrowseWidget( this ) )
, mpSLARTComWidget( new SLARTComWidget( this ) )
, mpTabs( new QTabWidget( this ) )
{
   QVBoxLayout *mainLayout = new QVBoxLayout( this );
   
   mpTabs->addTab( mpBrowseWidget,   tr("Filesystem") );
   mpTabs->addTab( mpSLARTComWidget, tr("Partyman") );
   
   QLabel *mpLogo = new QLabel( this );
   mpLogo->setText( QApplication::applicationName() );
   mpLogo->setAlignment( Qt::AlignCenter );
   mpLogo->setFrameShadow( QFrame::Raised );
   mpLogo->setFrameShape( QFrame::Box );

   mainLayout->addWidget( mpLogo );
   mainLayout->addWidget( mpTabs );
   
   connect( mpSLARTComWidget, SIGNAL( showInFilesystem(const QString&) ),
            mpBrowseWidget, SLOT(scrollTo(const QString&)) );
   connect( mpSLARTComWidget, SIGNAL( showInFilesystem(const QString&) ),
            this, SLOT( goToFilesystem() ) );
   
   setLayout( mainLayout );
}


bool MainWidget::shutdown()
{
   return true;
}


void MainWidget::goToFilesystem()
{
   mpTabs->setCurrentWidget( mpBrowseWidget );
}
