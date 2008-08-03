/**
 * MainWidget.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 */

#include "MainWidget.hpp"
#include "ButtonsWidget.hpp"
#include "MySettings.hpp"
#include "ConfigDialog.hpp"
#include "GlobalConfigWidget.hpp"
#include "ScrollLine.hpp"
#include "TrackInfoWidget.hpp"
#include "Database.hpp"

#include <QtGui>

#include "Trace.hpp"


MainWidget::MainWidget( QWidget *parent , Qt::WindowFlags flags )
: QWidget( parent, flags )
, mpDatabase( new Database() )
, mpFileName( new ScrollLine( this ) )
, mpTrackInfo( new TrackInfoWidget( mpDatabase, QString("k0u"), this ) )
, mpReadButton( new QPushButton( this ) )
, mpWriteButton( new QPushButton( this ) )
, mpListButtons( new ButtonsWidget( tr("Folders:"), this ) )
, mpSettingsButton( new QPushButton( tr("Settings"), this ) )
, mpAddButton( new QPushButton( tr("Add"), this ) )
, mpRemoveButton( new QPushButton( tr("Remove"), this ) )
, mpRemoveMenu( new QMenu( this ) )
, mpConfigDialog( new ConfigDialog( this ) )
, mpTimer( new QTimer( this ) )
, mSLARTCom()
, mPlaylists()
, mTrackInfo()
{
   QGridLayout *mainLayout   = new QGridLayout( this );
   
#if QT_VERSION < 0x040300
   mainLayout->setMargin( 3 );
#else
   mainLayout->setContentsMargins( 3, 3, 3, 3 );
#endif
   
   mpTimer->setSingleShot( true );
   mpTimer->setInterval( 2000 );
   
   QLabel *pLogo = new QLabel( this );
   pLogo->setText( QApplication::applicationName() );
   pLogo->setAlignment( Qt::AlignCenter );
   pLogo->setFrameShadow( QFrame::Raised );
   pLogo->setFrameShape( QFrame::Box );
   
   mpFileName->setReadOnly( true );
   mpRemoveButton->setMenu( mpRemoveMenu );

   mainLayout->addWidget( pLogo,            0, 0, 1, 6 );
   mainLayout->addWidget( mpFileName,       1, 0, 1, 6 );
   mainLayout->addWidget( mpTrackInfo,      2, 0, 1, 6 );
   mainLayout->addWidget( mpReadButton,     3, 0, 1, 3 );
   mainLayout->addWidget( mpWriteButton,    3, 3, 1, 3 );
   mainLayout->addWidget( mpListButtons,    4, 0, 1, 6 );
   mainLayout->addWidget( mpSettingsButton, 5, 0, 1, 2 );
   mainLayout->addWidget( mpAddButton,      5, 2, 1, 2 );
   mainLayout->addWidget( mpRemoveButton,   5, 4, 1, 2 );
   
   setLayout( mainLayout );
   
   updateLists();

   connect( &mSLARTCom, SIGNAL(packageRead(QStringList)),
            this, SLOT(handleSLART(QStringList)) );
            
   connect( mpSettingsButton, SIGNAL(clicked()),
            mpConfigDialog, SLOT(exec()) );
   connect( mpConfigDialog, SIGNAL(configChanged()),
            this, SLOT(labelReadWriteButtons()) );
   connect( mpConfigDialog, SIGNAL(configChanged()),
            this, SLOT(updateLists()) );
   connect( mpTimer, SIGNAL(timeout()),
            this, SLOT(sendK0u()) );
   
   connect( mpListButtons, SIGNAL(clicked(QWidget*)),
            this, SLOT(addToList(QWidget*)) );
   connect( mpAddButton, SIGNAL(clicked()),
            this, SLOT(handleAdd()) );
   connect( mpRemoveMenu, SIGNAL(triggered(QAction *)),
            this, SLOT(handleRemove(QAction *)) );
   connect( mpReadButton, SIGNAL(clicked()),
            this, SLOT(handleReadButton()) );
   connect( mpWriteButton, SIGNAL(clicked()),
            this, SLOT(handleWriteButton()) );

   labelReadWriteButtons();
   mSLARTCom.resetReceiver();
   
   MySettings().sendUdpMessage( "P0R", "Partyman" );
}


void MainWidget::addToList( QWidget *widget )
{
   QPushButton *pb = (QPushButton*)widget;
   if( mTrackInfo.mID > 0 )
   {
      mTrackInfo.setFolder( pb->text(), pb->isChecked() );
      mpDatabase->updateTrackInfo( &mTrackInfo );
      mpTimer->start();
   }
   else
   {
      pb->setChecked( false );
   }
}


void MainWidget::sendK0u()
{
      MySettings().sendNotification( QString("k0u") );
}


void MainWidget::handleSLART( const QStringList &message )
{
   if( message.count() > 1 )
   {
      if( message.at(0) == "p0p" )
      {
         mpFileName->setText( message.at(1) );
         mpDatabase->getTrackInfo( &mTrackInfo, message.at(1) );
         mpTrackInfo->getTrack( mTrackInfo );
         mpListButtons->lockButtons( mTrackInfo.getFolders() );
      }
   }
   
   if( (message.at(0) == "p0u") || 
       (message.at(0) == "r0u") )
   {
      mpDatabase->getTrackInfo( &mTrackInfo );
      mpTrackInfo->getTrack( mTrackInfo );
      mpListButtons->lockButtons( mTrackInfo.getFolders() );
   }
}


void MainWidget::updateLists()
{
   int i;
   mPlaylists = mpDatabase->getFolders();
   mPlaylists.sort();
   
   mpListButtons->updateButtons( mPlaylists );
   mpRemoveMenu->clear();
   
   for( i = 0; i < mPlaylists.count(); i++ )
   {
      mpRemoveMenu->addAction( mPlaylists.at(i) );
   }
}


void MainWidget::handleAdd()
{
   bool ok;
   QString folder( QInputDialog::getText( this, QApplication::applicationName(),
                                          QString( tr("Enter name of folder:") ),
                                          QLineEdit::Normal, QString(), &ok ) );
   if( ok && !folder.isEmpty() )
   {
      mpDatabase->insertFolder( folder );
      updateLists();
   }
}


void MainWidget::handleRemove( QAction *action )
{
   if( QMessageBox::Ok == QMessageBox::question( this, QApplication::applicationName(),
                                                 QString( tr("Are you sure you want to delete the folder\n") ) +
                                                 action->text(), QMessageBox::Ok | QMessageBox::Cancel ) )
   {
      mpDatabase->deleteFolder( action->text() );
      updateLists();
   }
}


void MainWidget::handleReadButton()
{
   mpFileName->setText( GlobalConfigWidget::getClipboard() );
   mpDatabase->getTrackInfo( &mTrackInfo, GlobalConfigWidget::getClipboard() );
   mpTrackInfo->getTrack( mTrackInfo );
}


void MainWidget::handleWriteButton()
{
   GlobalConfigWidget::setClipboard( mpFileName->text() );
}


void MainWidget::labelReadWriteButtons()
{
   MySettings settings( "Global" );
   int mode = settings.value( "ClipboardMode", 0 ).toInt();

   switch( mode )
   {
      case 1:
      case 3:
         mpReadButton->setText( tr("Read Selection") );
         mpReadButton->setHidden( false );
         break;
      case 2:
      case 4:
         mpReadButton->setText( tr("Read Clipboard") );
         mpReadButton->setHidden( false );
         break;
      default:
         mpReadButton->setText( QString() );
         mpReadButton->setHidden( true );
         break;
   }

   switch( mode )
   {
      case 1:
         mpWriteButton->setText( tr("Write Selection") );
         mpWriteButton->setHidden( false );
         break;
      case 2:
         mpWriteButton->setText( tr("Write Clipboard") );
         mpWriteButton->setHidden( false );
         break;
      case 3:
      case 4:
         mpWriteButton->setText( tr("Write Both") );
         mpWriteButton->setHidden( false );
         break;
      default:
         mpWriteButton->setText( QString() );
         mpWriteButton->setHidden( true );
         break;
   }
}
