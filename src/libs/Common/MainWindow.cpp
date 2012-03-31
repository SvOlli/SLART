/*
 * src/libs/Common/MainWindow.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#include "MainWindow.hpp"
#include "GlobalConfigWidget.hpp"
#include "PasswordChecker.hpp"
#include "Settings.hpp"

#include <QtGui>

#include "Trace.hpp"


MainWindow::MainWindow( bool saveWindow, QWidget *parent, Qt::WindowFlags flags )
: QMainWindow( parent, flags )
, mProhibitCloseWindow( false )
, mSaveWindow( saveWindow )
, mForbidMove( 75 )
, mpMainWidget( 0 )
{
   setWindowTitle( QApplication::applicationName() );
   QApplication::setDoubleClickInterval( Settings::value( Settings::GlobalDoubleClickInterval ) );
}


MainWindow::~MainWindow()
{
}


void MainWindow::setMainWidget( QWidget *mainWidget )
{
   mpMainWidget = mainWidget;
   setCentralWidget( mpMainWidget );
   restoreGeometry( Settings::value( Settings::CommonGeometry ) );
   restoreState( Settings::value( Settings::CommonState ) );
   QList<QDockWidget*> docks( findChildren<QDockWidget*>() );
   foreach( QDockWidget *dock, docks )
   {
      restoreDockWidget( dock );
   }
}


/* Evil workaround: in the first fifty events revert all move requests */
bool MainWindow::event( QEvent *event )
{
   if( mForbidMove > 0 )
   {
      if( (event->type() == QEvent::Move ) && mSaveWindow )
      {
         restoreGeometry( Settings::value( Settings::CommonGeometry ) );
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
      Settings::setValue( Settings::CommonGeometry, saveGeometry() );
      Settings::setValue( Settings::CommonState, saveState() );
   }
   event->accept();
}
