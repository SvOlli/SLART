/**
 * src/libs/Common/MainWindowCheckClose.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

/* class declaration */
#include "MainWindowCheckClose.hpp"

/* system headers */

/* Qt headers */
#include <QCloseEvent>

/* local library headers */

/* local headers */
#include "PasswordChecker.hpp"


MainWindowCheckClose::MainWindowCheckClose( bool saveWindow,
                                            QWidget *parent, Qt::WindowFlags flags )
: MainWindow( saveWindow, parent, flags )
, mProhibitCloseWindow( false )
{
}


MainWindowCheckClose::~MainWindowCheckClose()
{
}


void MainWindowCheckClose::closeEvent( QCloseEvent *event )
{
   if( mProhibitCloseWindow )
   {
      if( !(PasswordChecker::get()->unlock()) )
      {
         event->ignore();
         return;
      }
   }
   MainWindow::closeEvent( event );
}
