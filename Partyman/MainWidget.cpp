/**
 * MainWidget.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 */

#include "MainWidget.hpp"
#include "ConfigDialog.hpp"
#include "ControlWidget.hpp"
#include "PlaylistWidget.hpp"
#include "MySettings.hpp"

#include <QtGui>

#include <signal.h>


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
, mpConfig( new ConfigDialog( this ) )
, mpPlaylist( new PlaylistWidget( mpConfig, this ) )
, mpControl( new ControlWidget( mpConfig, mpPlaylist, this ) )
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

   ::signal( SIGHUP,  signalHandler );
   ::signal( SIGINT,  signalHandler );
   ::signal( SIGQUIT, signalHandler );
   ::signal( SIGILL,  signalHandler );
   ::signal( SIGABRT, signalHandler );
   ::signal( SIGSEGV, signalHandler );
   ::signal( SIGPIPE, signalHandler );
   ::signal( SIGTERM, signalHandler );
   ::signal( SIGPWR,  signalHandler );
   
   connect( mpControl, SIGNAL(requestAddToPlaylist(QStringList,bool)), 
            mpPlaylist, SLOT(addEntries(QStringList,bool)) );
   connect( mpControl, SIGNAL(requestChangeTitle(QIcon,QString)),
            this, SLOT(changeTitle(QIcon,QString)) );
   connect( mpSettingsButton, SIGNAL(clicked()),
            mpConfig, SLOT(exec()) );
   connect( mpConfig, SIGNAL(configChanged()),
            mpPlaylist, SLOT(readM3u()) );
   connect( mpPlaylist, SIGNAL(playlistIsValid(bool)),
            mpControl, SLOT(allowConnect(bool)) );
   connect( mpPlaylist, SIGNAL(playlistIsValid(bool)),
            this, SLOT(allowAutostart(bool)) );
            
   setLayout( mainLayout );
   
   mpSettingsButton->setMaximumWidth( fontMetrics().width( mpSettingsButton->text() ) + 16 );
   mpSettingsButton->setMaximumHeight( fontMetrics().height() + 8 );
   mpSettingsButton->raise();
}


MainWidget::~MainWidget()
{
   mpMainWidget = 0;
   ::signal( SIGHUP,  SIG_DFL );
   ::signal( SIGINT,  SIG_DFL );
   ::signal( SIGQUIT, SIG_DFL );
   ::signal( SIGILL,  SIG_DFL );
   ::signal( SIGABRT, SIG_DFL );
   ::signal( SIGSEGV, SIG_DFL );
   ::signal( SIGPIPE, SIG_DFL );
   ::signal( SIGTERM, SIG_DFL );
   ::signal( SIGPWR,  SIG_DFL );

   delete mpControl;
   mpControl = 0;
   delete mpPlaylist;
   mpPlaylist = 0;
}

void MainWidget::changeTitle( const QIcon &icon, const QString &title )
{
   emit requestChangeTitle( icon, title );
}


void MainWidget::startUp()
{
   MySettings settings;
   mpPlaylist->readM3u();
   mpControl->readConfig();
   if( !QFileInfo( settings.value( "DatabaseFilename", "" ).toString() ).isFile() )
   {
      mpConfig->exec();
   }
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
}
