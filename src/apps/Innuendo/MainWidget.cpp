/**
 * src/apps/Innuendo/MainWidget.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "MainWidget.hpp"

/* system headers */

/* Qt headers */
#include <QtGui>

/* local library headers */
#include <GenericSatMsgHandler.hpp>
#include <GlobalConfigWidget.hpp>
#include <MySettings.hpp>
#include <Satellite.hpp>
#include <WidgetShot.hpp>

/* local headers */
#include "ConfigDialog.hpp"
#include "ExecButton.hpp"
#include "DropDialog.hpp"
#include "LogListWidget.hpp"


MainWidget::MainWidget( QWidget *parent, Qt::WindowFlags flags )
: QWidget( parent, flags )
, mpSatellite( Satellite::get( this ) )
, mpGenericSatMsgHandler( new GenericSatMsgHandler( mpSatellite, GenericSatMsgHandler::WithPingAndDialog ) )
, mpMessageBuffer( new LogListWidget( this ) )
, mpSettingsButton( new QPushButton( tr("Settings"), this ) )
, mpPingButton( new QPushButton( tr("Ping"), this ) )
, mpConfig( new ConfigDialog( this ) )
, mpDropDialog( new DropDialog( this, Qt::WindowTitleHint | Qt::WindowSystemMenuHint ) )
, mpExecButtons(0)
, mNumExecButtons(0)
, mAutostart( MySettings().VALUE_STARTUP )
{
   QGridLayout *mainLayout   = new QGridLayout( this );
   mainLayout->setContentsMargins( 3, 3, 3, 3 );
   parent->setWindowIcon( QIcon( ":/SLART.png" ) );

   mpMessageBuffer->setDragEnabled( true );

   QStringList applications;
   applications
      << "Partyman"
      << "Karmadrome"
      << "Rubberbandman"
      << "Stripped"
      << "Funkytown"
      << "Notorious"
      << "Creep"
      ;
   QStringList toolTips;
   toolTips
      << "Audio Player"
      << "Playlist Organizer"
      << "Tag Editor"
      << "CD Ripper"
      << "Downloader"
      << "FreeDB Search Tool"
      << "Remote Control Daemon"
      ;

   mNumExecButtons = applications.count();
   mpExecButtons = new ExecButton*[mNumExecButtons];

   mainLayout->addWidget( mpMessageBuffer,   0, 0, 1, 3 );
   for( int i = 0; i < mNumExecButtons; i++ )
   {
      mpExecButtons[i] = new ExecButton( applications.at(i), this );
      mpExecButtons[i]->setToolTip( toolTips.at(i) );
      mainLayout->addWidget( mpExecButtons[i], 1 + i/3, i%3 );
   }
   mainLayout->addWidget( mpSettingsButton,  2 + (mNumExecButtons-1)/3, 0, 1, 2 );
   mainLayout->addWidget( mpPingButton,      2 + (mNumExecButtons-1)/3, 2 );

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
   connect( mpGenericSatMsgHandler, SIGNAL(anotherInstance()),
            this, SLOT(noAutostart()) );
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
   WidgetShot::addWidget( "MainWidget", this );
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


void MainWidget::noAutostart()
{
   mAutostart.clear();
}


void MainWidget::readConfig()
{
   mpSatellite->restart();
}


void MainWidget::handlePingButton()
{
   mpSatellite->send( "PNG" );
}


void MainWidget::handleSatellite( const QByteArray &msg )
{
   QListWidgetItem *item = new QListWidgetItem( QDateTime::currentDateTime().toString(), mpMessageBuffer );
   item->setBackground( QBrush( mpMessageBuffer->palette().color( QPalette::AlternateBase ) ) );
   mpMessageBuffer->addItem( item );

   foreach( const QString &line, Satellite::split( msg ) )
   {
      mpMessageBuffer->addItem( line );
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
