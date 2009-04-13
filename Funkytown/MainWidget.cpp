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

#include "Trace.hpp"

MainWidget::MainWidget( QWidget *parent )
: QWidget( parent )
, mpLayout( new QGridLayout( this ) )
, mpDirText( new QLabel( tr("Target Dir:"), this ) )
, mpDirButton( new QPushButton( this ) )
, mpNameText( new QLabel( tr("URL:"), this ) )
, mpNameInput( new QLineEdit( this ) )
, mpGoButton( new QPushButton( this ) )
, mpSetupButton( new QPushButton( tr("Settings / Log / Help"), this ) )
, mpConfigDialog( new ConfigDialog( this ) )
, mpDownloadHandler( new DownloadHandler() )
{
   MySettings settings;
   //mpNameInput->setText("http://www.myspace.com/hertzinfarkt");
   
#if QT_VERSION < 0x040300
   mpLayout->setMargin( 3 );
#else
   mpLayout->setContentsMargins( 3, 3, 3, 3 );
   mpLayout->setSpacing( 3 );
#endif
   
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
   mpDownloadHandler->setAcceptDrops( false );
   
   mpDirButton->setText( settings.VALUE_DIRECTORY );
   QDir::setCurrent( mpDirButton->text() );
   
   downloadActive( false );
   
   mpLayout->addWidget( mpLogo,              0, 0, 1, 2 );
   
   mpLayout->addWidget( mpDirText,           1, 0 );
   mpLayout->addWidget( mpDirButton,         1, 1 );
   
   mpLayout->addWidget( mpNameText,          2, 0 );
   mpLayout->addWidget( mpNameInput,         2, 1 );
   
   mpLayout->addWidget( mpGoButton,          3, 0, 1, 2 );
   mpLayout->addWidget( mpDownloadHandler,   4, 0, 1, 2 );
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
   connect( mpDownloadHandler, SIGNAL(downloadActive(bool)),
            this, SLOT(downloadActive(bool)) );
   connect( mpDownloadHandler, SIGNAL(errorMessage(const QString&)),
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
   
   QDir::setCurrent( mpDirButton->text() );
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
   
   mpDownloadHandler->run( url );
   
   mpNameInput->setText("");
}


void MainWidget::downloadActive( bool active )
{
   mpDirButton->setDisabled( active );
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
