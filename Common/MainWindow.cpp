/**
 * MainWindow.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#include "MainWindow.hpp"
#include "MainWidget.hpp"
#include "MySettings.hpp"

#include <QtGui>

#include "Trace.hpp"


MainWindow::MainWindow( QWidget *parent, Qt::WindowFlags flags )
: QMainWindow( parent, flags )
, mpMainWidget( new MainWidget( this ) )
{
   setCentralWidget( mpMainWidget );
   setWindowTitle( QApplication::applicationName() );
   
   MySettings().setMainWindow( this );

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
   MySettings().saveMainWindow( this );
   event->accept();
}


bool MainWindow::invokeSetUp( const char *argv0 )
{
   QProcess sorcerer;
   QDir qdir( argv0 );
   qdir.cdUp();
   sorcerer.start( qdir.absolutePath() + "/Sorcerer" );
   if( sorcerer.waitForStarted() )
   {
      sorcerer.waitForFinished( -1 );
      return sorcerer.exitCode() == 0;
   }
   else
   {
      sorcerer.terminate();
      return false;
   }
}
