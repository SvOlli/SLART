/**
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
   // TODO: move to MainWidget
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
   return QWidget::event( event );
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


#if MAINWINDOW_SORCERER
bool MainWindow::invokeSetUp( QApplication *app )
{
   QDir sorcererDir(qApp->applicationDirPath());
#if defined Q_OS_MAC
   if( sorcererDir.dirName() == "MacOS" )
   {
      QDir searchDir( sorcererDir );
      searchDir.cdUp();
      if( searchDir.cd( "PlugIns" ) )
      {
         if( trySorcerer( app, searchDir ) )
         {
            return true;
         }
      }
   }
#elif defined Q_OS_UNIX
   if( sorcererDir.dirName() == "bin" )
   {
      QDir searchDir( sorcererDir );
      searchDir.cdUp();
      if( searchDir.cd( "lib" ) )
      {
         if( trySorcerer( app, searchDir ) )
         {
            return true;
         }
      }
   }
#endif
   if( trySorcerer( app, sorcererDir ) )
   {
      return true;
   }
   
   QMessageBox::critical( 0, app->applicationName(),
                          tr("Setup failed!\nCannot start.\nSorry.") );
   return false;
}


bool MainWindow::trySorcerer( QApplication *app, const QDir &dir )
{
   int retval = 1;
   QString path( dir.absolutePath() );
#if defined Q_OS_WIN
   path.append( "/Sorcerer.dll" );
#elif defined Q_OS_MAC
   path.append( "/libSorcerer.dylib" );
#else
   path.append( "/libSorcerer.so" );
#endif
   QPluginLoader pluginLoader( path );
   QObject *plugin = pluginLoader.instance();
   if( plugin )
   {
      SorcererInterface *sorcerer = qobject_cast<SorcererInterface *>(plugin);
      if( sorcerer )
      {
         retval = sorcerer->run( app );
      }
   }
   
   return retval == 0;
}
#endif
