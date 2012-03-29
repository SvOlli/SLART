/*
 * src/libs/Common/MainWindow.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#include "MainWindow.hpp"
#include "MySettings.hpp"
#include "GlobalConfigWidget.hpp"
#include "PasswordChecker.hpp"

#include <QtGui>

#include "Trace.hpp"
#include "WidgetShot.hpp"


MainWindow::MainWindow( bool saveWindow, QWidget *parent, Qt::WindowFlags flags )
: QMainWindow( parent, flags )
, mProhibitCloseWindow( false )
, mSaveWindow( saveWindow )
, mForbidMove( 50 )
, mpMainWidget( 0 )
{
   setWindowTitle( QApplication::applicationName() );
   QApplication::setDoubleClickInterval( MySettings( "Global" ).VALUE_DOUBLECLICKINTERVAL );

/*
   // \todo: move to MainWidget
   connect( mpMainWidget, SIGNAL(requestChangeTitle(QIcon,QString)),
            this, SLOT(changeTitle(QIcon,QString)) );
   connect( mpMainWidget, SIGNAL(kioskMode(bool)),
            this, SLOT(prohibitClose(bool)) );
 */

}


MainWindow::~MainWindow()
{
}


void MainWindow::setMainWidget( QWidget *mainWidget )
{
   mpMainWidget = mainWidget;
   setCentralWidget( mpMainWidget );
   if( mSaveWindow )
   {
      MySettings().setMainWindow( this );
   }
}


void MainWindow::enableScreenshot()
{
   WidgetShot::addWidget( "MainWidget", mpMainWidget );
}


/* Evil workaround: in the first fifty events revert all move requests */
bool MainWindow::event( QEvent *event )
{
   if( mForbidMove > 0 )
   {
      if( (event->type() == QEvent::Move ) && mSaveWindow )
      {
         MySettings().setMainWindow( this );
      }
      mForbidMove--;
   }
   return QMainWindow::event( event );
}


void MainWindow::changeTitle( const QIcon &icon, const QString &title )
{
   if( !icon.isNull() )
   {
      setWindowIcon( icon );
   }
   if( !title.isNull() )
   {
      setWindowTitle( title );
   }
}


void MainWindow::closeEvent( QCloseEvent *event )
{
   if( mSaveWindow )
   {
      MySettings().saveMainWindow( this );
   }

   event->accept();
}
