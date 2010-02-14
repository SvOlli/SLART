/**
 * src/apps/Innuendo/MainWidget.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#include "MainWidget.hpp"

#include <QtGui>
#include "MySettings.hpp"
#include "ConfigDialog.hpp"
#include "GenericSatMsgHandler.hpp"
#include "GlobalConfigWidget.hpp"
#include "ExecButton.hpp"
#include "DropDialog.hpp"
#include "MySettings.hpp"
#include "Satellite.hpp"

#include "Trace.hpp"


MainWidget::MainWidget( QWidget *parent, Qt::WindowFlags flags )
: QWidget( parent, flags )
, mpSatellite( Satellite::get( this ) )
, mpGenericSatMsgHandler( new GenericSatMsgHandler( mpSatellite ) )
, mpMessageBuffer( new QListWidget( this ) )
, mpSettingsButton( new QPushButton( tr("Settings"), this ) )
, mpPingButton( new QPushButton( tr("Ping"), this ) )
, mpConfig( new ConfigDialog( this ) )
, mpDropDialog( new DropDialog( this, Qt::WindowTitleHint | Qt::WindowSystemMenuHint ) )
, mpExecButtons(0)
, mNumExecButtons(0)
, mAutostart( MySettings().VALUE_STARTUP )
{
   QGridLayout *mainLayout   = new QGridLayout( this );
#if QT_VERSION < 0x040300
   mainLayout->setMargin( 3 );
#else
   mainLayout->setContentsMargins( 3, 3, 3, 3 );
#endif
   parent->setWindowIcon( QIcon( ":/SLART.png" ) );

   QStringList applications;
   applications
      << "Partyman"
      << "Stripped"
      << "Funkytown"
      << "Rubberbandman"
      << "Karmadrome"
      << "Creep"
      ;
   QStringList toolTips;
   toolTips 
      << "Audio Player"
      << "CD Ripper"
      << "MySpace Downloader"
      << "Tag Editor"
      << "Playlist Organizer"
      << "Remote Control Daemon"
      ;
   
   mNumExecButtons = applications.count();
   mpExecButtons = new ExecButton*[mNumExecButtons];
   
   mainLayout->addWidget( mpMessageBuffer,   0, 0, 1, mNumExecButtons );
   for( int i = 0; i < mNumExecButtons; i++ )
   {
      mpExecButtons[i] = new ExecButton( applications.at(i), this );
      mpExecButtons[i]->setToolTip( toolTips.at(i) );
      mainLayout->addWidget( mpExecButtons[i], 1, i );
   }
   mainLayout->addWidget( mpSettingsButton,  2, 0 );
   mainLayout->addWidget( mpPingButton,      2, 1 );

   connect( mpSettingsButton, SIGNAL(clicked()),
            mpConfig, SLOT(exec()) );
   connect( mpPingButton, SIGNAL(clicked()),
            this, SLOT(handlePingButton()) );
   connect( mpConfig, SIGNAL(configChanged()),
            this, SLOT(readConfig()) );
   connect( mpSatellite, SIGNAL(received(const QByteArray &)),
            this, SLOT(handleSatellite(const QByteArray &)) );
   connect( mpGenericSatMsgHandler, SIGNAL(updateConfig()),
            mpConfig, SLOT(readSettings()) );
#if SATELLITE_DEBUG
   connect( mpSatellite, SIGNAL(debug(const QByteArray &)),
            this, SLOT(handleSatellite(const QByteArray &)) );
#endif
   connect( mpMessageBuffer, SIGNAL(itemClicked(QListWidgetItem*)),
            this, SLOT(listWidgetItemToClipboard(QListWidgetItem*)) );

   readConfig();
   
   setLayout( mainLayout );
   
   mpSettingsButton->setObjectName( QString("SettingsButton") );
   
   setAcceptDrops( true );
   
   QTimer::singleShot(333, this, SLOT(autostart()));
}


MainWidget::~MainWidget()
{
   delete[] mpExecButtons;
}


#if 0
void MainWidget::aboutToClose()
{
TRACESTART(MainWidget::closeEvent)
   QStringList runningApplications;
   
   for( int i = 0; i < mNumExecButtons; i++ )
   {
TRACEMSG << mpExecButtons[i]->text() << mpExecButtons[i]->isChecked();
      if( mpExecButtons[i]->isChecked() )
      {
         runningApplications << mpExecButtons[i]->text();
         mpExecButtons[i]->click();
      }
   }
   if( runningApplications.count() )
   {
      MySettings().setValue( "Startup", runningApplications );
   }
   else
   {
      MySettings().remove( "Startup" );
   }
}
#endif


void MainWidget::autostart()
{
   if( mAutostart.count() > 0 )
   {
      for( int i = 0; i < mNumExecButtons; i++ )
      {
         if( mAutostart.contains( mpExecButtons[i]->text() ) )
         {
            mpExecButtons[i]->click();
            mAutostart.removeAt( mAutostart.indexOf( mpExecButtons[i]->text() ) );
            QTimer::singleShot(500, this, SLOT(autostart()));
            break;
         }
      }
   }
}


void MainWidget::readConfig()
{
   mpSatellite->restart();
}


void MainWidget::handlePingButton()
{
   MySettings settings;
   mpMessageBuffer->addItem( "** pinging applications\n(not reimplemented yet)" );
#if 0
   mpPingButton->setDisabled( true );
   for( int i = 0; i < mNumExecButtons; i++ )
   {
      QString msg("* ");
      msg.append( mpExecButtons[i]->text() );
      if( mSLARTCom.ping( mpExecButtons[i]->text() ) )
      {
         msg.append( " has reponded" );
      }
      else
      {
         msg.append( " is not responing" );
      }
      mpMessageBuffer->addItem( msg );
      mpMessageBuffer->scrollToBottom();
   }
   mpPingButton->setDisabled( false );
#endif
}


void MainWidget::handleSatellite( const QByteArray &message )
{
   QListWidgetItem *item = new QListWidgetItem( QDateTime::currentDateTime().toString(), mpMessageBuffer );
   item->setBackground( QBrush( mpMessageBuffer->palette().color( QPalette::AlternateBase ) ) );
   mpMessageBuffer->addItem( item );

   QStringList lines( Satellite::split( message ) );
   for( int i = 0; i < lines.size(); i++ )
   {
      mpMessageBuffer->addItem( lines.at(i) );
   }

   while( mpMessageBuffer->count() > MySettings().VALUE_BUFFERSIZE )
   {
      item = mpMessageBuffer->takeItem(0);
      if( item )
      {
         delete item;
      }
   }
   
   mpMessageBuffer->scrollToBottom();
}


void MainWidget::listWidgetItemToClipboard( QListWidgetItem *item )
{
   GlobalConfigWidget::setClipboard( item->text() );
}


void MainWidget::dragEnterEvent( QDragEnterEvent *event )
{
   event->acceptProposedAction();
}


void MainWidget::dropEvent( QDropEvent *event )
{
   mpDropDialog->dropEvent( event );
   mpDropDialog->show();
}
