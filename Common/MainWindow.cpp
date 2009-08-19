/**
 * MainWindow.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#include "MainWindow.hpp"
#include "MainWidget.hpp"
#include "MySettings.hpp"
#include "../Sorcerer/Interface.hpp"
#include "GlobalConfigWidget.hpp"
#if MAINWINDOW_PROHIBITCLOSE
#include "ConfigDialog.hpp"
#endif

#include <QtGui>

#include "Trace.hpp"
#include "WidgetShot.hpp"


MainWindow::MainWindow( bool saveWindow, QWidget *parent, Qt::WindowFlags flags )
: QMainWindow( parent, flags )
#if MAINWINDOW_PROHIBITCLOSE
, mProhibitCloseWindow( false )
#endif
, mSaveWindow( saveWindow )
, mpMainWidget( new MainWidget( this ) )
{
   setCentralWidget( mpMainWidget );
   setWindowTitle( QApplication::applicationName() );
   QApplication::setDoubleClickInterval( MySettings( "Global" ).VALUE_DOUBLECLICKINTERVAL );
   
   if( mSaveWindow )
   {
      MySettings().setMainWindow( this );
   }

#if MAINWINDOW_CHANGETITLE
   connect( mpMainWidget, SIGNAL(requestChangeTitle(QIcon,QString)),
            this, SLOT(changeTitle(QIcon,QString)) );
#endif
#if MAINWINDOW_PROHIBITCLOSE
   connect( mpMainWidget, SIGNAL(kioskMode(bool)),
            this, SLOT(prohibitClose(bool)) );
#endif
#if MAINWIDGET_SCREENSHOT
   WidgetShot::addWidget( "MainWidget", mpMainWidget );
#endif
}


#if MAINWINDOW_CHANGETITLE
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
#endif


MainWidget *MainWindow::mainWidget()
{
   return mpMainWidget;
}


void MainWindow::closeEvent( QCloseEvent *event )
{
#if MAINWINDOW_PROHIBITCLOSE
   if( mProhibitCloseWindow )
   {
      if( !ConfigDialog::checkPassword() )
      {
         event->ignore();
         return;
      }
   }
#endif
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
      if( searchDir.cd( "Plugins" ) )
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
