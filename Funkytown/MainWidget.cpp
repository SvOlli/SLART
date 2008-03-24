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
#include "ConfigDialog.hpp"
#include "MySettings.hpp"
#include "DownloadHandler.hpp"
#include "PostDownloadHandlerHTML.hpp"
#include "PostDownloadHandlerXML.hpp"
#include "PostDownloadHandlerMP3.hpp"
#include "PostDownloadHandlerFLV.hpp"
#include "GlobalHandlers.hpp"

#include "Trace.hpp"

MainWidget::MainWidget( QWidget *parent )
: QWidget( parent )
, mpLayout( new QGridLayout( this ) )
, mpDirText( new QLabel( tr("Target Dir:"), this ) )
, mpDirButton( new QPushButton( this ) )
, mpNameText( new QLabel( tr("URL:"), this ) )
, mpNameInput( new QLineEdit( this ) )
, mpGoButton( new QPushButton( this ) )
, mpSetupButton( new QPushButton( tr("Settings / Log"), this ) )
, mpConfigDialog( new ConfigDialog( this ) )
{
   char cwd[PATH_MAX];
   MySettings settings;
   
   gpDownloadHandler         = new DownloadHandler();
   gpPostDownloadHandlerHTML = new PostDownloadHandlerHTML();
   gpPostDownloadHandlerXML  = new PostDownloadHandlerXML();
   gpPostDownloadHandlerMP3  = new PostDownloadHandlerMP3();
   gpPostDownloadHandlerFLV  = new PostDownloadHandlerFLV();
   
   QLabel *mpLogo = new QLabel( this );
   mpLogo->setText( QApplication::applicationName() );
   mpLogo->setAlignment( Qt::AlignCenter );
   mpLogo->setFrameShadow( QFrame::Raised );
   mpLogo->setFrameShape( QFrame::Box );

   mpDirText->setAcceptDrops( false );
   mpDirButton->setAcceptDrops( false );
   mpNameText->setAcceptDrops( false );
   mpNameInput->setAcceptDrops( false );
   mpGoButton->setAcceptDrops( false );
   mpSetupButton->setAcceptDrops( false );
   gpDownloadHandler->setAcceptDrops( false );
   
   QVariant targetDir( getcwd(&cwd[0], PATH_MAX) );
   mpDirButton->setText( settings.value("Directory", targetDir).toString() );
   chdir( mpDirButton->text().toLocal8Bit().constData() );
   
   downloadActive( false );
   
   mpLayout->addWidget( mpLogo,              0, 0, 1, 2 );
   
   mpLayout->addWidget( mpDirText,           1, 0 );
   mpLayout->addWidget( mpDirButton,         1, 1 );
   
   mpLayout->addWidget( mpNameText,          2, 0 );
   mpLayout->addWidget( mpNameInput,         2, 1 );
   
   mpLayout->addWidget( mpGoButton,          3, 0, 1, 2 );
   mpLayout->addWidget( gpDownloadHandler,   4, 0, 1, 2 );
   mpLayout->addWidget( mpSetupButton,       5, 0, 1, 2 );
   
   setLayout( mpLayout );
   
   connect( mpDirButton, SIGNAL(clicked()),
            this, SLOT(setDownloadDir()) );
   connect( mpGoButton,  SIGNAL(clicked()),
            this, SLOT(downloadUserPage()) );
   connect( mpNameInput, SIGNAL(returnPressed()),
            this, SLOT(downloadUserPage()) );
   connect( mpSetupButton, SIGNAL(clicked()),
            mpConfigDialog, SLOT(exec()) );
   connect( gpDownloadHandler, SIGNAL(downloadActive(bool)),
            this, SLOT(downloadActive(bool)) );
   connect( gpDownloadHandler, SIGNAL(errorMessage(const QString&)),
            mpConfigDialog, SLOT(logMessage(const QString&)) );
   
   setAcceptDrops( true );
}


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
   QString url( mpNameInput->text() );
   
   if( url.size() == 0 )
      return;

   downloadActive( true );
   gpDownloadHandler->setProxy( mpConfigDialog->proxyWidget() );
   
   if( url.endsWith( ".mp3", Qt::CaseInsensitive ) )
   {
      QString fileName( url.mid( url.lastIndexOf( '/' ) + 1 ) );
      gpDownloadHandler->run( url, fileName, gpPostDownloadHandlerMP3 );
   }
   else
   {
      gpDownloadHandler->run( url,
                              QString("webpage.html"),
                              gpPostDownloadHandlerHTML );
   }
   
   mpNameInput->setText("");
}


void MainWidget::downloadActive( bool active )
{
   mpDirButton->setDisabled( active );
   mpConfigDialog->proxyWidget()->updateWidgets( active );
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
