/*
 * src/tests/TestAppSatellite/MainWindow.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

/* class declaration */
#include "MainWindow.hpp"

/* system headers */

/* Qt headers */
#include <QCoreApplication>

/* local library headers */

/* local headers */
#include "MainWidget.hpp"



MainWindow::MainWindow( QWidget *parent, Qt::WindowFlags flags )
: QMainWindow( parent, flags )
{
   setCentralWidget( new MainWidget( this ) );
   setWindowTitle( QCoreApplication::applicationName() );
}
