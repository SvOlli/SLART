/**
 * MainWindow.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#include <QtGui>
#include "MainWindow.hpp"

#include "BrowseWidget.hpp"


MainWindow::MainWindow( QWidget * parent, Qt::WindowFlags flags )
: QMainWindow( parent, flags )
, mpBrowseWidget( new BrowseWidget() )
{
   mpTabs = new QTabWidget();
   setCentralWidget( mpTabs );

   mpTabs->addTab( mpBrowseWidget,   tr("Browse") );
}


bool MainWindow::shutdown()
{
   return true;
}

