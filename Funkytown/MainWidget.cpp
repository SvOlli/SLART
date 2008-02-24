/**
 * MainWidget.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#include <unistd.h>

#include <QtGui>
#include <QtNetwork>
#include "MainWidget.hpp"
#include "DownloadHandler.hpp"
#include "PostDownloadHandlerHTML.hpp"
#include "PostDownloadHandlerXML.hpp"
#include "PostDownloadHandlerMP3.hpp"
#include "SetupDialog.hpp"
#include "MySettings.hpp"
#include "Trace.hpp"

#include "Version.hpp"


MainWidget::MainWidget( QWidget *parent )
: QWidget( parent )
, mpLayout( new QGridLayout )
, mpDirText( new QLabel )
, mpDirButton( new QPushButton )
, mpNameText( new QLabel )
, mpNameInput( new QLineEdit )
, mpGoButton( new QPushButton )
, mpDownloadHandler( new DownloadHandler )
, mpSetupButton( new QPushButton )
, mpPostDownloadHandlerHTML( new PostDownloadHandlerHTML )
, mpPostDownloadHandlerXML( new PostDownloadHandlerXML )
, mpPostDownloadHandlerMP3( new PostDownloadHandlerMP3 )
, mpSetupDialog( new SetupDialog( this ) )
{
   char cwd[PATH_MAX];
   MySettings settings;
   
   mpDirText->setAcceptDrops( false );
   mpDirButton->setAcceptDrops( false );
   mpNameText->setAcceptDrops( false );
   mpNameInput->setAcceptDrops( false );
   mpGoButton->setAcceptDrops( false );
   mpSetupButton->setAcceptDrops( false );
   mpDownloadHandler->setAcceptDrops( false );
   
   // connect the dots
   mpPostDownloadHandlerHTML->setHandlers( mpDownloadHandler, mpPostDownloadHandlerXML );
   mpPostDownloadHandlerXML->setHandlers( mpDownloadHandler, mpPostDownloadHandlerMP3 );
   mpPostDownloadHandlerMP3->setHandlers( mpDownloadHandler, mpPostDownloadHandlerMP3 );
   
   QVariant targetDir( getcwd(&cwd[0], PATH_MAX) );
   mpDirText->setText( tr("Target Dir:") );
   mpDirButton->setText( settings.value("Directory", targetDir).toString() );
   chdir( mpDirButton->text().toLocal8Bit().constData() );
   
   mpNameText->setText( tr("MySpace Name/URL:") );
//   mpNameInput->setText( tr("") );
   downloadActive( false );
   mpSetupButton->setText( tr("Setup / Log") );
   
   mpLayout->addWidget( mpDirText,           0, 0 );
   mpLayout->addWidget( mpDirButton,         0, 1 );
   
   mpLayout->addWidget( mpNameText,          1, 0 );
   mpLayout->addWidget( mpNameInput,         1, 1 );
   
   mpLayout->addWidget( mpGoButton,          2, 0, 1, 2 );
   mpLayout->addWidget( mpDownloadHandler,   3, 0, 1, 2 );
   mpLayout->addWidget( mpSetupButton,       4, 0, 1, 2 );
   
   setLayout( mpLayout );
   
   connect( mpDirButton, SIGNAL(clicked()), this, SLOT(setDownloadDir()) );
   connect( mpGoButton,  SIGNAL(clicked()), this, SLOT(downloadUserPage()) );
   connect( mpNameInput, SIGNAL(returnPressed()), this, SLOT(downloadUserPage()) );
   connect( mpSetupButton, SIGNAL(clicked()), this, SLOT(showSetupDialog()) );
   connect( mpDownloadHandler, SIGNAL(downloadActive(bool)), this, SLOT(downloadActive(bool)) );
   connect( mpDownloadHandler, SIGNAL(errorMessage(const QString&)), mpSetupDialog, SLOT(logMessage(const QString&)) );
//   if( parent )
//   {
//      connect( parent, SIGNAL(aboutToQuit()), mpSetupDialog, SLOT(gotCloseSignal()) );
//   }
   setAcceptDrops( true );
}


MainWidget::~MainWidget()
{
}

#if 0
void MainWidget::gotCloseSignal()
{
}
#endif


void MainWidget::setDownloadDir()
{
   QFileDialog fileDialog( this );
   
   fileDialog.setFileMode( QFileDialog::DirectoryOnly );
   fileDialog.setDirectory( mpDirButton->text() );
   fileDialog.setReadOnly( false );
   
   if( fileDialog.exec() )
   {
      MySettings settings;
      QString result( fileDialog.selectedFiles().at(0) );
      mpDirButton->setText( result );
      settings.setValue( "Directory", result.replace('\\','/') );
   }
   
   chdir( mpDirButton->text().toLocal8Bit().constData() );
}


void MainWidget::downloadUserPage( const QString &name )
{
   if( !name.isEmpty() )
   {
      mpNameInput->setText( name );
   }
   
   if( mpNameInput->text().size() == 0 )
      return;

   downloadActive( true );
   mpDownloadHandler->setProxy( mpSetupDialog->proxyWidget() );
   
   mpDownloadHandler->run( mpNameInput->text(),
//                           mpNameInput->text().replace(QRegExp("^.*/"),"")+QString(".html"),
                           QString("webpage.html"),
                           mpPostDownloadHandlerHTML );
   
   mpNameInput->setText("");
}


void MainWidget::showSetupDialog()
{
   mpSetupButton->setDisabled( true );
   QCoreApplication::processEvents();
   mpSetupDialog->proxyWidget()->readSettings();
   mpSetupDialog->exec();
   mpSetupDialog->proxyWidget()->writeSettings();
   mpSetupButton->setDisabled( false );
}


void MainWidget::downloadActive( bool active )
{
   mpDirButton->setDisabled( active );
   mpSetupDialog->proxyWidget()->updateWidgets( active );
   if( active )
   {
      mpGoButton->setText( tr("ADD!") );
   }
   else
   {
      mpGoButton->setText( tr("GO!") );
   }
}


void MainWidget::dragEnterEvent( QDragEnterEvent *event )
{
   if( event->mimeData()->hasFormat("text/plain") )
   {
      event->acceptProposedAction();
   }
}


void MainWidget::dropEvent( QDropEvent *event )
{
   const QMimeData *mimeData = event->mimeData();
   
   if( mimeData->hasText() )
   {
      QString url( mimeData->text() );
      
      if( url.startsWith("http://") )
      {
         downloadUserPage( url.replace( QRegExp("\n.*$"),"") );
      }
   }
}
