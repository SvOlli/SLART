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

#include <QtGui>

#include "Trace.hpp"


MainWindow::MainWindow( bool saveWindow, QWidget *parent, Qt::WindowFlags flags )
: QMainWindow( parent, flags )
, mSaveWindow( saveWindow )
, mpMainWidget( new MainWidget( this ) )
{
   setCentralWidget( mpMainWidget );
   setWindowTitle( QApplication::applicationName() );
   
   if( mSaveWindow )
   {
      MySettings().setMainWindow( this );
   }

   connect( mpMainWidget, SIGNAL(requestChangeTitle(QIcon,QString)),
            this, SLOT(changeTitle(QIcon,QString)) );
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


MainWidget *MainWindow::mainWidget()
{
   return mpMainWidget;
}


void MainWindow::closeEvent( QCloseEvent *event )
{
   if( mSaveWindow )
   {
      MySettings().saveMainWindow( this );
   }
   event->accept();
}


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
   
   if( retval )
   {
      QMessageBox::critical( 0, app->applicationName(), tr("Setup failed!\nCannot start.\nSorry.") );
      return false;
   }
   return true;
}
