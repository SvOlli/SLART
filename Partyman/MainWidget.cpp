/**
 * MainWidget.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 */

#include "MainWidget.hpp"
#include "Database.hpp"
#include "ConfigDialog.hpp"
#include "ControlWidget.hpp"
#include "PlaylistWidget.hpp"
#include "MySettings.hpp"

#include <QtGui>

#include <signal.h>

#include "Trace.hpp"

static MainWidget *mpMainWidget;


static void signalHandler( int signum )
{
   if( mpMainWidget ) delete mpMainWidget;
   
   raise( signum );
}


MainWidget::MainWidget( QWidget *parent , Qt::WindowFlags flags )
: QWidget( parent, flags )
, mAllowAutostart( false )
, mpParent( parent )
, mpDatabase( new Database() )
, mpConfig( new ConfigDialog( mpDatabase, this ) )
, mpPlaylist( new PlaylistWidget( mpDatabase, mpConfig, this ) )
, mpControl( new ControlWidget( mpDatabase, mpConfig, mpPlaylist, this ) )
, mpSettingsButton( new QPushButton( tr("Settings"), this ) )
{
   mpMainWidget = this;
   QVBoxLayout *mainLayout   = new QVBoxLayout( this );
   
#if QT_VERSION < 0x040300
   mainLayout->setMargin( 3 );
#else
   mainLayout->setContentsMargins( 3, 3, 3, 3 );
#endif
   mainLayout->addWidget( mpControl );
   mainLayout->addWidget( mpPlaylist );
   mpParent->setAttribute( Qt::WA_AlwaysShowToolTips, true );
   mpParent->setWindowIcon( QIcon( ":/PartymanSmile.gif" ) );

#if (!defined _WIN32) || (defined __MINGW32__)
   int i;
   for( i = 1; i < 32; i++ )
   {
      switch( i )
      {
         // numeric because of MinGW lacking most signals
         case  9: //SIGKILL
         case 13: //SIGPIPE
         case 17: //SIGCHLD
         case 19: //SIGSTOP
            break;
         default:
            ::signal( i, signalHandler );
            break;
      }
   }
#endif
   
   connect( mpControl, SIGNAL(requestAddToPlaylist(QStringList,bool)), 
            mpPlaylist, SLOT(addEntries(QStringList,bool)) );
   connect( mpControl, SIGNAL(requestChangeTitle(QIcon,QString)),
            this, SLOT(changeTitle(QIcon,QString)) );
   connect( mpSettingsButton, SIGNAL(clicked()),
            mpConfig, SLOT(exec()) );
   connect( mpPlaylist, SIGNAL(playlistIsValid(bool)),
            mpControl, SLOT(allowConnect(bool)) );
   connect( mpPlaylist, SIGNAL(playlistIsValid(bool)),
            this, SLOT(allowAutostart(bool)) );
   connect( mpControl, SIGNAL(trackUpdate()), 
            mpPlaylist, SLOT(updateTrackInfo()) );
   
   setLayout( mainLayout );
   
   mpSettingsButton->setMaximumWidth( fontMetrics().width( mpSettingsButton->text() ) + 16 );
   mpSettingsButton->setMaximumHeight( fontMetrics().height() + 8 );
   mpSettingsButton->raise();
}


MainWidget::~MainWidget()
{
   mpMainWidget = 0;
#if (!defined _WIN32) || (defined __MINGW32__)
   int i;
   for( i = 1; i < 32; i++ )
   {
      switch( i )
      {
         // numeric because of MinGW lacking most signals
         case  9: //SIGKILL
         case 13: //SIGPIPE
         case 17: //SIGCHLD
         case 19: //SIGSTOP
            break;
         default:
            ::signal( i, SIG_DFL );
            break;
      }
   }
#endif

   delete mpControl;
   mpControl = 0;
   delete mpPlaylist;
   mpPlaylist = 0;
   delete mpDatabase;
   mpDatabase = 0;
}


void MainWidget::changeTitle( const QIcon &icon, const QString &title )
{
   emit requestChangeTitle( icon, title );
}


void MainWidget::startUp()
{
   MySettings settings;
   mpControl->readConfig();
   mpPlaylist->startBrowserUpdate();
   QCoreApplication::processEvents();
   if( settings.value( "AutoConnect", false ).toBool() && mAllowAutostart )
   {
      mpControl->initConnect();
   }
   else
   {
      mpControl->initDisconnect();
   }
}


void MainWidget::resizeEvent( QResizeEvent *event )
{
   QWidget::resizeEvent( event );
   
   mpSettingsButton->move( width()  - mpSettingsButton->width() - 3,
                           height() - mpSettingsButton->height() );
}


void MainWidget::allowAutostart( bool allow )
{
   mAllowAutostart = allow;
   if( !allow )
   {
      QMessageBox::warning( this, QApplication::applicationName()+tr(": database empty"), 
                            QString(tr("The database is empty. Please run Rubberbandman.")) );
   }
}
