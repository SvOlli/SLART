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
, mpSLATComWidget( new SLATComWidget( this ) )
{
   QVBoxLayout *mainLayout = new QVBoxLayout( this );
   
   mpTabs = new QTabWidget( this );

   mpTabs->addTab( mpBrowseWidget,   tr("Browse") );
   mpTabs->addTab( mpSLATComWidget,  tr("Partyman") );
   
   QLabel *mpLogo = new QLabel( this );
   mpLogo->setText( QApplication::applicationName() );
   mpLogo->setAlignment( Qt::AlignCenter );
   mpLogo->setFrameShadow( QFrame::Raised );
   mpLogo->setFrameShape( QFrame::Box );

   mainLayout->addWidget( mpLogo );
   mainLayout->addWidget( mpTabs );
}


bool MainWidget::shutdown()
{
   return true;
}

