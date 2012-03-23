/*
 * src/apps/Stripped/StrippedMainWindow.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#include "StrippedMainWindow.hpp"
#include "StrippedMainWidget.hpp"
#include "Settings.hpp"
#include "GlobalConfigWidget.hpp"
#include "PasswordChecker.hpp"

#include <QtGui>

#include "Trace.hpp"
#include "WidgetShot.hpp"


StrippedMainWindow::StrippedMainWindow( QWidget *parent, Qt::WindowFlags flags )
: QMainWindow( parent, flags )
, mForbidMove( 50 )
, mpMainWidget( new StrippedMainWidget( this ) )
{
   setWindowTitle( QApplication::applicationName() );
   QApplication::setDoubleClickInterval( Settings::get( "Global" )->VALUE_DOUBLECLICKINTERVAL );
   setCentralWidget( mpMainWidget );

   QSettings *settings = Settings::get();
   restoreGeometry( settings->value("Geometry").toByteArray() );
   restoreState( settings->value("State").toByteArray() );
   QList<QDockWidget*> docks( findChildren<QDockWidget*>() );
   foreach( QDockWidget *dock, docks )
   {
      restoreDockWidget( dock );
   }
   WidgetShot::addWidget( "MainWidget", mpMainWidget );
}


StrippedMainWindow::~StrippedMainWindow()
{
}


/* Evil workaround: in the first fifty events revert all move requests */
bool StrippedMainWindow::event( QEvent *event )
{
   if( mForbidMove > 0 )
   {
      if( event->type() == QEvent::Move )
      {
         restoreGeometry( Settings::get()->value("Geometry").toByteArray() );
      }
      mForbidMove--;
   }
   return QWidget::event( event );
}


void StrippedMainWindow::changeTitle( const QIcon &icon, const QString &title )
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


void StrippedMainWindow::closeEvent( QCloseEvent *event )
{
   QSettings *settings = Settings::get();
   settings->setValue( "Geometry", saveGeometry() );
   settings->setValue( "State", saveState() );
   event->accept();
}
