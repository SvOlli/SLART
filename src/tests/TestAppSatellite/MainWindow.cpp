/**
 * MainWindow.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

/* class declaration */
#include "MainWindow.hpp"

/* system headers */

/* Qt headers */
#include <QtGui>

/* local library headers */

/* local headers */
#include "MainWidget.hpp"



MainWindow::MainWindow( QWidget *parent, Qt::WindowFlags flags )
: QMainWindow( parent, flags )
{
   setCentralWidget( new MainWidget( this ) );
   setWindowTitle( QApplication::applicationName() );
}
