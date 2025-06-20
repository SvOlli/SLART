/*
 * src/apps/Partyman/ControlWidget.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "ControlWidget.hpp"

/* system headers */
#include <csignal>

/* Qt headers */
#include <QApplication>
#include <QCommonStyle>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QGridLayout>
#include <QMenu>
#include <QMessageBox>
#include <QMimeData>
#include <QPushButton>
#include <QToolButton>
#include <QUrl>

/* local library headers */
#include <CrashCleanup.hpp>
#include <GenericSatelliteHandler.hpp>
#include <GlobalConfigWidget.hpp>
#include <Satellite.hpp>
#include <Settings.hpp>
#include <WidgetShot.hpp>

/* local headers */
#include "PartymanConfigDialog.hpp"
#include "PartymanMainWindow.hpp"


ControlWidget::ControlWidget( Database *database, PartymanConfigDialog *config,
                              PartymanMainWindow *parent )
: QWidget( parent )
, mpConfig( config )
, mpPlaylist( parent )
, mpSatellite( Satellite::get() )
, mpGenericSatelliteHandler( 0 )
, mPartymanIcon( QIcon( ":/Partyman/Icon.png" ) )
, mStopIcon( QCommonStyle().standardIcon(QStyle::SP_MediaStop) /*QIcon(":/Stop.png")*/ )
, mPlayIcon( QCommonStyle().standardIcon(QStyle::SP_MediaPlay) /*QIcon(":/Play.png")*/ )
, mPauseIcon( QCommonStyle().standardIcon(QStyle::SP_MediaPause) /*QIcon(":/Pause.png")*/ )
, mSkipIcon( QCommonStyle().standardIcon(QStyle::SP_MediaSkipForward) /*QIcon(":/Skip.png")*/ )
, mLoadIcon( QIcon( ":/Partyman/Load.png" ) )
, mpSettingsButton( new QPushButton( tr("Settings"), this ) )
, mpStartButton( new QToolButton( /*tr("Connect"),*/ this ) )
, mpSkipButton( new QToolButton( /*mSkipIcon, tr("Next"),*/ this ) )
, mpTrayIcon( new QSystemTrayIcon( this ) )
, mpTrayIconStopMenu( new QMenu( this ) )
, mpTrayIconPlayMenu( new QMenu( this ) )
, mpStartButtonMenu( new QMenu( mpStartButton ) )
, mpPlayAction( mpTrayIconStopMenu->addAction( mPlayIcon, tr("Start" ) ) )
, mpSkipAction( mpTrayIconPlayMenu->addAction( mSkipIcon, tr("Next" ) ) )
, mpPauseAction( mpStartButtonMenu->addAction( mPauseIcon, tr("Pause" ) ) )
, mpStopAction( mpStartButtonMenu->addAction( mStopIcon, tr("Disconnect" ) ) )
, mpLoadAction( mpStartButtonMenu->addAction( mLoadIcon, tr("Load" ) ) )
, mpTrayIconClickTimer( new QTimer( this ) )
, mConnected( false )
, mPaused( false )
, mKioskMode( false )
, mpDerMixDprocess( new QProcess( this ) )
, mpLoggerProcess( new QProcess( this ) )
, mWaitForDerMixD( false )
, mDerMixDstarted( false )
, mLastP0p()
{
   CrashCleanup::addObject( mpDerMixDprocess );
   setAcceptDrops( true );
   setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed );
   mpSettingsButton->setObjectName( QString("SettingsButton") );
   mpPlayer[0] = new PlayerWidget( 0, database, this );
   mpPlayer[1] = new PlayerWidget( 1, database, this );
   mpTrayIconClickTimer->setSingleShot( true );

   QGridLayout *mainLayout    = new QGridLayout( this );

   mainLayout->setContentsMargins( 3, 3, 3 ,3 );
   mainLayout->setSpacing( 5 );
   mainLayout->addWidget( mpPlayer[0],      1, 0, 4, 1 );
   mainLayout->addWidget( mpPlayer[1],      1, 2, 4, 1 );
   mainLayout->addWidget( mpSettingsButton, 1, 1 );
   mainLayout->addWidget( mpStartButton,  3, 1 );
   mainLayout->addWidget( mpSkipButton,     4, 1 );

   mainLayout->setColumnStretch( 0, 1 );
   mainLayout->setColumnStretch( 2, 1 );
   mainLayout->setRowStretch( 0, 1 );
   mainLayout->setRowStretch( 5, 1 );

   setLayout( mainLayout );

   mpStartButton->setDefaultAction( mpPlayAction );
   mpStartButton->setPopupMode( QToolButton::InstantPopup );
   mpStartButton->setToolButtonStyle( Qt::ToolButtonTextBesideIcon );
   mpStartButton->setSizePolicy( QSizePolicy::Minimum, QSizePolicy::Fixed );
   mpPlayAction->setCheckable( true );
   mpPlayAction->setDisabled( true );

   mpSkipButton->setDefaultAction( mpSkipAction );
   mpSkipButton->setPopupMode( QToolButton::InstantPopup );
   mpSkipButton->setToolButtonStyle( Qt::ToolButtonTextBesideIcon );
   mpSkipButton->setSizePolicy( QSizePolicy::Minimum, QSizePolicy::Fixed );
   mpSkipAction->setCheckable( true );
   mpSkipAction->setDisabled( true );

   mpTrayIcon->setIcon( QIcon(":/Partyman/Icon.png") );
   mpTrayIcon->setContextMenu( mpTrayIconStopMenu );
   mpTrayIconPlayMenu->addAction( mpPauseAction );
   mpTrayIconPlayMenu->addAction( mpStopAction );

   connect( mpSettingsButton, SIGNAL(clicked()),
            mpConfig, SLOT(exec()) );
   connect( mpStartButton, SIGNAL(clicked()),
            this, SLOT(initConnect()) );
   connect( mpPlayAction, SIGNAL(triggered()),
            this, SLOT(initConnect()) );
   connect( mpPauseAction, SIGNAL(triggered()),
            this, SLOT(handlePause()) );
   connect( mpStopAction, SIGNAL(triggered()),
            this, SLOT(initDisconnect()) );
   connect( mpLoadAction, SIGNAL(triggered()),
            this, SLOT(handleLoad()) );
   connect( mpSkipAction, SIGNAL(triggered()),
            this, SLOT(handleSkipTrack()) );
   connect( mpConfig, SIGNAL(configChanged()),
            this, SLOT(readConfig()) );
   connect( mpPlayer[0], SIGNAL(trackPlaying(TrackInfo)),
            this, SLOT(handleTrackPlaying(TrackInfo)) );
   connect( mpPlayer[1], SIGNAL(trackPlaying(TrackInfo)),
            this, SLOT(handleTrackPlaying(TrackInfo)) );
   connect( mpTrayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(handleTrayIcon(QSystemTrayIcon::ActivationReason)) );
   connect( mpTrayIconClickTimer, SIGNAL(timeout()),
            this, SLOT(handlePause()) );

   connect( mpDerMixDprocess, SIGNAL(readyReadStandardError()),
            this, SLOT(handleDerMixDstartup()) );
   connect( mpDerMixDprocess, SIGNAL(finished(int,QProcess::ExitStatus)),
            this, SLOT(handleDerMixDfinish(int,QProcess::ExitStatus)) );
   connect( mpDerMixDprocess, SIGNAL(error(QProcess::ProcessError)),
            this, SLOT(handleDerMixDerror(QProcess::ProcessError)) );

   if( mpSatellite )
   {
      mpGenericSatelliteHandler = new GenericSatelliteHandler( mpSatellite, GenericSatelliteHandler::WithPingAndDialog, this );
      connect( mpGenericSatelliteHandler, SIGNAL(updateConfig()),
               mpConfig, SLOT(readSettings()) );
      connect( mpGenericSatelliteHandler, SIGNAL(anotherInstance()),
               this, SLOT(initDisconnect()) );
      connect( mpSatellite, SIGNAL(received(QByteArray)),
               this, SLOT(handleSatellite(QByteArray)) );
   }
}


ControlWidget::~ControlWidget()
{
   mpPlayer[0]->disconnect();
   mpPlayer[1]->disconnect();

   if( mpDerMixDprocess->state() != QProcess::NotRunning )
   {
      mpDerMixDprocess->terminate();
      mpDerMixDprocess->waitForFinished();
   }
   if( mpDerMixDprocess->state() != QProcess::NotRunning )
   {
      mpDerMixDprocess->kill();
      mpDerMixDprocess->waitForFinished();
   }
   CrashCleanup::removeObject( mpDerMixDprocess );
   Settings::setValue( Settings::PartymanDerMixDpid, 0 );
}


void ControlWidget::saveTracks( bool unload )
{
   int i;
   QString current;
   QString next;

   for( i = 0; i < 2; i++ )
   {
      switch( mpPlayer[i]->getState() )
      {
         case PlayerFSM::searching:
         case PlayerFSM::loading:
         case PlayerFSM::ready:
            next = mpPlayer[i]->getCurrentTrack();
            break;
         case PlayerFSM::playing:
         case PlayerFSM::paused:
            current = mpPlayer[i]->getCurrentTrack();
            break;
         default:
            break;
      }
   }

   if( unload )
   {
      if( !next.isEmpty() )
      {
         mpPlaylist->addEntries( QStringList( next ), true );
      }
      if( !current.isEmpty() )
      {
         mpPlaylist->addEntries( QStringList( current ), true );
      }
      mpPlaylist->setCurrentNext();
   }
   else
   {
      mpPlaylist->setCurrentNext( current, next );
   }
   mpPlaylist->savePlaylist();
}


void ControlWidget::setActions()
{
   if( !mConnected )
   {
      mpPlayAction->setChecked( true );
      mpSkipAction->setDisabled( true );
      mpStartButton->setMenu( 0 );
      mpStartButton->setChecked( false );
   }
   else
   {
      mpStartButton->setMenu( mpStartButtonMenu );
      mpStartButton->setChecked( true );

      if( mPaused )
      {
         mpPauseAction->setIcon( mPlayIcon );
         mpPauseAction->setText( tr("Resume") );
      }
      else
      {
         if( mKioskMode )
         {
            mpSkipAction->setIcon( QIcon() );
            mpSkipAction->setText( tr("Kiosk Mode") );
            mpSkipAction->setEnabled( false );
            mpPlayAction->setEnabled( false );
            mpPauseAction->setEnabled( false );
         }
         else
         {
            mpSkipAction->setIcon( mSkipIcon );
            mpSkipAction->setText( tr("Next") );
            mpSkipAction->setEnabled( true );
            mpPlayAction->setEnabled( true );
            mpPauseAction->setEnabled( true );

            if( Settings::value( Settings::PartymanDerMixDrun ) )
            {

            }
            else
            {

            }
         }
      }
   }

   mpPlayAction->setCheckable( true );
   mpPlayAction->setChecked( true );
   mpPlayAction->setDisabled( true );

   mpSkipAction->setDisabled( true );

   mpStartButton->setMenu( true ? 0 : mpStartButtonMenu );
   mpStartButton->setChecked( false );

   mpStopAction->setDisabled( mKioskMode );

   mpPauseAction->setDisabled( false );

   mpPlayAction->setIcon( mPlayIcon );
   mpPlayAction->setText( tr("Start") );
   mpStopAction->setIcon( mStopIcon );
   mpStopAction->setText( tr("Stop") );


   if( !mConnected )
   {
      mpPlayAction->setChecked( true );
      mpSkipAction->setDisabled( true );
      mpStartButton->setMenu( 0 );
      mpStartButton->setChecked( false );
   }
   else
   {
      mpSkipAction->setDisabled( mKioskMode );
      mpPauseAction->setDisabled( mKioskMode & !mPaused );
      mpStopAction->setDisabled( mKioskMode );
      mpSkipAction->setDisabled( mKioskMode | !mConnected );
      if( mKioskMode )
      {
         mpSkipAction->setIcon( QIcon() );
         mpSkipAction->setText( tr("Kiosk Mode") );
      }
      else
      {
         mpSkipAction->setIcon( mSkipIcon );
         mpSkipAction->setText( tr("Next") );
      }

      if( mPaused )
      {
         mpPlayAction->setIcon( mPauseIcon );
         mpPlayAction->setText( tr("Pause") );
         mpPauseAction->setIcon( mPlayIcon );
         mpPauseAction->setText( tr("Resume") );
      }
      else
      {
         if( Settings::value( Settings::PartymanDerMixDrun ) )
         {
            mpPlayAction->setIcon( mPlayIcon );
            mpPlayAction->setText( tr("Start") );
            mpStopAction->setIcon( mStopIcon );
            mpStopAction->setText( tr("Stop") );
         }
         else
         {
            mpPlayAction->setIcon( QIcon() );
            mpPlayAction->setText( tr("Connect") );
            mpStopAction->setIcon( QIcon() );
            mpStopAction->setText( tr("Disconnect") );
         }
      }
   }
   mpLoadAction->setEnabled( Settings::value( Settings::GlobalClipboardMode ) > 0 );
}


void ControlWidget::readConfig()
{
   mpPlayer[0]->readConfig();
   mpPlayer[1]->readConfig();
   if( Settings::value( Settings::PartymanDerMixDrun ) )
   {
      mpPlayAction->setIcon( mPlayIcon );
      mpPlayAction->setText( tr("Start") );
      mpStopAction->setIcon( mStopIcon );
      mpStopAction->setText( tr("Stop") );
   }
   else
   {
      mpPlayAction->setIcon( QIcon() );
      mpPlayAction->setText( tr("Connect") );
      mpStopAction->setIcon( QIcon() );
      mpStopAction->setText( tr("Disconnect") );
   }
   mpTrayIcon->setVisible( Settings::value( Settings::PartymanTrayIcon ) &&
                           QSystemTrayIcon::isSystemTrayAvailable() );
   mpLoadAction->setEnabled( Settings::value( Settings::GlobalClipboardMode ) > 0 );
}


void ControlWidget::addToPlaylist( const QStringList &entries )
{
   emit requestAddToPlaylist( entries, true );
}


void ControlWidget::initConnect()
{
   if( !mConnected )
   {
      QString hostname( "localhost" );
      int port = Settings::value( Settings::PartymanDerMixDport );

      if( Settings::value( Settings::PartymanDerMixDrun ) )
      {
         QStringList args;
         QString params( Settings::value( Settings::PartymanDerMixDparams ) );
         args << "-c" << QString("main.port=%1").arg( port );
         if( !params.isEmpty() )
         {
            args << params.split(' ');
         }
         qlonglong pid = Settings::value( Settings::PartymanDerMixDpid );
         if( pid > 0 )
         {
#ifdef Q_OS_UNIX
            ::kill( static_cast<pid_t>( pid ), SIGTERM );
#endif
         }
         mpDerMixDprocess->start( Settings::value( Settings::PartymanDerMixDcmd ), args );
         /* block until dermixd is up an running */
         for( mWaitForDerMixD = true; mWaitForDerMixD; )
         {
            QCoreApplication::processEvents();
         }
         if( !mDerMixDstarted )
         {
            QMessageBox::critical( this, QApplication::applicationName(),
                                   tr("Could not start DerMixD") );
         }
         pid = static_cast<qlonglong>( mpDerMixDprocess->processId() );
         Settings::setValue( Settings::PartymanDerMixDpid, pid );
      }
      else
      {
         mDerMixDstarted = true;
         hostname = Settings::value( Settings::PartymanDerMixDhost );
      }
      mConnected = true;
      mpPlayer[0]->connectTo( hostname, port );
      mpPlayer[1]->connectTo( hostname, port );
      mpStartButton->setMenu( mpStartButtonMenu );
      mpTrayIcon->setContextMenu( mpTrayIconPlayMenu );
      mpPlayAction->setChecked( true );
      mpSkipAction->setDisabled( mKioskMode );
      emit signalConnected( true );
   }
   handlePause( true );
}


void ControlWidget::initDisconnect( ErrorCode errorCode )
{
   PartymanMainWindow::setIconAndTitle( this, mPartymanIcon, QApplication::applicationName() +
                                                             ": " + tr("(disconnected)") );
   mpTrayIcon->setToolTip( QString() );
   mpTrayIcon->setContextMenu( mpTrayIconStopMenu );
   if( mConnected )
   {
      mConnected = false;
      QString errorText;
      handlePause( true );
      saveTracks( true );
      mpPlayer[0]->disconnect();
      mpPlayer[1]->disconnect();
      //mpSkipButton->setDisabled( true );
      mpSkipAction->setDisabled( true );
      mpStartButton->setMenu( 0 );
      mpStartButton->setChecked( false );
      switch( errorCode )
      {
         case ErrorNoConnection:
            errorText = tr("Could not connect to DerMixD. Is it running?");
            break;
         case ErrorConnectionLost:
            errorText = tr("The connection to DerMixD was lost.");
            break;
         case ErrorWrongVersion:
            errorText = tr("The Version of DerMixD is not compatible.");
            break;
         case ErrorNone:
         default:
            break;
      }
      if( !errorText.isEmpty() && mDerMixDstarted )
      {
         QMessageBox::critical( this, QApplication::applicationName(), errorText );
      }
      if( Settings::value( Settings::PartymanDerMixDrun ) )
      {
         mpDerMixDprocess->terminate();
         Settings::setValue( Settings::PartymanDerMixDpid, 0 );
      }
      emit signalConnected( false );
      mpPlaylist->setTrackInfo( TrackInfo() );
      log( "p0s", "stop" );
   }
}


void ControlWidget::handlePause( bool reset )
{
   if( mpPlayer[1]->getState() == PlayerFSM::endingpaused )
   {
      mpPlayer[1]->pause();
      mpPlayer[0]->pause();
   }
   else
   {
      mpPlayer[0]->pause();
      mpPlayer[1]->pause();
   }
   if( mPaused || reset )
   {
      mpPlayAction->setIcon( mPlayIcon );
      mpPlayAction->setText( tr("Start") );
      mpPauseAction->setIcon( mPauseIcon );
      mpPauseAction->setText( tr("Pause") );
      if( mKioskMode )
      {
         mpPauseAction->setDisabled( true );
      }
      mPaused = false;
   }
   else
   {
      PartymanMainWindow::setIconAndTitle( this, mPauseIcon, QApplication::applicationName() +
                                                             ": " + tr("(paused)") );
      mpPlayAction->setIcon( mPauseIcon );
      mpPlayAction->setText( tr("Pause") );
      mpPauseAction->setIcon( mPlayIcon );
      mpPauseAction->setText( tr("Resume") );
      if( mKioskMode )
      {
         mpPauseAction->setDisabled( false );
      }
      mPaused = true;
   }
   mpStartButton->setMenu( mpStartButtonMenu );
}


void ControlWidget::handleLoad()
{
   mpPlaylist->addEntries( GlobalConfigWidget::getClipboard().split(QRegExp("(\r|\n)"),Qt::SkipEmptyParts) );
}


void ControlWidget::handleSkipTrack()
{
   if( mpSkipAction->isEnabled() )
   {
      bool checkstate = true;

      mpSkipButton->clearFocus();
      if( (mpPlayer[0]->getState() == PlayerFSM::ready) ||
          (mpPlayer[1]->getState() == PlayerFSM::ready) )
      {
         log( "p0n", "skip" );
         if( mPaused )
         {
            handlePause( true );
         }
         mpPlayer[0]->disablePlayPosition( true );
         mpPlayer[1]->disablePlayPosition( true );
         mpPlayer[0]->skip();
         mpPlayer[1]->skip();
         checkstate = false;
      }

      mpSkipAction->setChecked( checkstate );
   }
}


bool ControlWidget::isSkipChecked()
{
   return mpSkipButton->isChecked();
}


void ControlWidget::getNextTrack( QString *fileName )
{
   mpPlaylist->getNextTrack( fileName );
}


void ControlWidget::allowConnect( bool allowed )
{
   mpPlayAction->setDisabled( !allowed );
}


void ControlWidget::handleSatellite( const QByteArray &msg )
{
   QStringList src( Satellite::split( msg ) );

   if( src.size() > 0 )
   {
      if( src.at(0) == "P0Q" )
      {
         QStringList dest;

         /* convert from url to filename if necessary */
         for( int i = 1; i < src.size(); i++ )
         {
            QFileInfo qfi( src.at(i) );
            if( qfi.isFile() )
            {
               dest << qfi.absoluteFilePath();
            }
         }

         if( dest.size() > 0 )
         {
            emit requestAddToPlaylist( dest, false );
         }
      }

      else if( src.at(0) == "P0A" )
      {
         if( mConnected )
         {
            if( mPaused || !mKioskMode )
            {
               handlePause();
            }
         }
         else
         {
            initConnect();
         }
      }

      else if( src.at(0) == "P0N" )
      {
         handleSkipTrack();
      }

      else if( src.at(0) == "P0S" )
      {
         if( !mKioskMode )
         {
            initDisconnect();
         }
      }

      else if( src.at(0) == "P0R" )
      {
         if( mpSatellite && !mLastP0p.isEmpty() )
         {
            mpSatellite->send( mLastP0p );
         }
      }

      else if( src.at(0) == "P0C" )
      {
         bool favorite = false;
         bool unwanted = false;
         if( src.size() > 1 )
         {
            switch( src.at(1).toUInt() )
            {
               case 1:
                  favorite = true;
                  break;
               case 2:
                  unwanted = true;
                  break;
               default:
                  break;
            }
            mpPlaylist->setTrackInfoFavoriteUnwanted( favorite, unwanted );
         }
      }

      else if( src.at(0) == "P0T" )
      {
         if( src.size() > 1 )
         {
            emit requestTab( src.at(1) );
         }
      }

      else if( (src.at(0) == "k0u") ||
               (src.at(0) == "r0u") )
      {
         emit trackUpdate();
      }

      else if( src.at(0) == "u0p" )
      {
         if( Settings::value( Settings::PartymanStopOnUnderpassStart ) && !mKioskMode )
         {
            initDisconnect();
         }
      }
   }
}


void ControlWidget::log( const QString &udpEvent, const QString &logEvent, const QString &data )
{
   QByteArray msg( udpEvent.toUtf8() );
   if( !data.isEmpty() )
   {
      msg.append( '\n' );
      msg.append( data.toUtf8() );
   }
   if( mpSatellite )
   {
      mpSatellite->send( msg );
   }
   if( udpEvent == "p0p" )
   {
      mLastP0p = msg;
   }
   if( udpEvent == "p0s" )
   {
      mLastP0p.clear();
   }

   QString command( Settings::value( Settings::PartymanLogCmd ) );
   if( !command.isEmpty() )
   {
      QStringList args;
      args << logEvent;
      if( !data.isEmpty() )
      {
         args << data;
      }
      mpLoggerProcess->start( command, args );
   }
}


void ControlWidget::handleDerMixDstartup()
{
   QStringList data( QString::fromLocal8Bit( mpDerMixDprocess->readAllStandardError().constData() ).split('\n') );

   foreach( const QString &line, data )
   {
      int version = line.indexOf( " DerMixD v" );
      if( version > 0 )
      {
         /* this version of Partyman relies on DerMixD v2.1 or later */
         if( (line.mid( version + 10 /* " DerMixD v" */, 2 ) != "2.") ||
             (line.mid( version + 12 /* " DerMixD v2." */, 1 ) != "0"))
         {
            initDisconnect( ErrorWrongVersion );
         }
      }
      else if(( line.indexOf( " online on port " ) > 0 ) ||
         ( line.indexOf( " listen on port " ) > 0 ) )
      {
         mDerMixDstarted = true;
         mWaitForDerMixD = false;
      }
   }
}


void ControlWidget::handleDerMixDfinish( int /*exitCode*/,
                                         QProcess::ExitStatus /*exitStatus*/ )
{
   mDerMixDstarted = false;
   mWaitForDerMixD = false;
}


void ControlWidget::handleDerMixDerror( QProcess::ProcessError /*error*/ )
{
   mDerMixDstarted = false;
   mWaitForDerMixD = false;
}


void ControlWidget::changeOtherState( int player, PlayerFSM::tState state )
{
   mpPlayer[1-player]->setState( state );
}


void ControlWidget::handleTrackPlaying( const TrackInfo &trackInfo )
{
   /* pass through to track info widget */
   mpPlaylist->setTrackInfo( trackInfo );
   QString title( trackInfo.displayString( Settings::value( Settings::PartymanNamePattern ) ) );
   QString bubble( trackInfo.displayString( Settings::value( Settings::PartymanTrayIconPattern ) ) );
   if( trackInfo.mTitle.isEmpty() )
   {
      title  = QApplication::applicationName();
      bubble = trackInfo.mDirectory;
      bubble.append( '\n' );
      bubble.append( trackInfo.mFileName );
   }
   PartymanMainWindow::setIconAndTitle( this, mPlayIcon, title );
   mpTrayIcon->setToolTip( bubble );
   if( Settings::value( Settings::PartymanTrayIcon ) &&
       Settings::value( Settings::PartymanTrayIconBubble ) &&
       QSystemTrayIcon::supportsMessages() )
   {
      mpTrayIcon->showMessage( tr("Now Playing:"), bubble,
         (QSystemTrayIcon::MessageIcon)(Settings::value( Settings::PartymanTrayIconBubbleIcon ) ),
         (int)(Settings::value( Settings::PartymanTrayIconBubbleTime ) * 1000) );
   }
}

void ControlWidget::handleTrayIcon( QSystemTrayIcon::ActivationReason reason )
{
   if( reason == QSystemTrayIcon::Trigger )
   {
      if( mConnected )
      {
         if( !mKioskMode | mPaused )
         {
            mpTrayIconClickTimer->start( QApplication::doubleClickInterval() );
         }
      }
      else
      {
         initConnect();
      }
   }
   if( reason == QSystemTrayIcon::DoubleClick )
   {
      mpTrayIconClickTimer->stop();
      handleSkipTrack();
   }
}


void ControlWidget::handleKioskMode( bool enable )
{
   mKioskMode = enable;
   mpPlayer[0]->handleKioskMode( mKioskMode );
   mpPlayer[1]->handleKioskMode( mKioskMode );
   mpSkipAction->setDisabled( mKioskMode );
   mpPauseAction->setDisabled( mKioskMode & !mPaused );
   mpStopAction->setDisabled( mKioskMode );
   mpSkipAction->setDisabled( mKioskMode | !mConnected );
   if( mKioskMode )
   {
      mpSkipAction->setIcon( QIcon() );
      mpSkipAction->setText( tr("Kiosk Mode") );
   }
   else
   {
      mpSkipAction->setIcon( mSkipIcon );
      mpSkipAction->setText( tr("Next") );
   }
}


void ControlWidget::allowSkip()
{
   mpSkipAction->setChecked( false );
}


void ControlWidget::dragEnterEvent( QDragEnterEvent *event )
{
   const QMimeData *mimeData = event->mimeData();
   if( mimeData->hasUrls() )
   {
      PlayerFSM::eState state[2];
      state[0] = mpPlayer[0]->getState();
      state[1] = mpPlayer[1]->getState();
      if( (state[0] == PlayerFSM::disconnected) || (state[0] == PlayerFSM::ready) ||
          (state[1] == PlayerFSM::disconnected) || (state[1] == PlayerFSM::ready) )
      {
         // FIXME: prevent from dropping track already in one of two players
         event->acceptProposedAction();
         return;
      }
   }
   event->ignore();
}


void ControlWidget::dropEvent( QDropEvent *event )
{
   const QMimeData *mimeData = event->mimeData();
   if( mimeData->hasUrls() )
   {
      int player = -1;
      PlayerFSM::eState state[2];
      state[0] = mpPlayer[0]->getState();
      state[1] = mpPlayer[1]->getState();
      QString fileName;
      QStringList fileNames;
      foreach( const QUrl &url, mimeData->urls() )
      {
         fileName = url.toLocalFile();
         if( !fileName.isEmpty() )
         {
            fileNames.append( fileName );
         }
      }
      if( state[0] == PlayerFSM::ready )
      {
         player = 0;
      }
      else if( state[1] == PlayerFSM::ready )
      {
         player = 1;
      }
      if( player >= 0 )
      {
         fileNames << mpPlayer[player]->getCurrentTrack();
         addToPlaylist( fileNames );
         mpPlayer[player]->unload( true );
      }
      else
      {
         addToPlaylist( fileNames );
      }
      if( (state[0] == PlayerFSM::disconnected) &&
          (state[1] == PlayerFSM::disconnected) )
      {
         QMetaObject::invokeMethod( mpStartButton, "click",
                                    Qt::QueuedConnection );
      }
      event->acceptProposedAction();
      return;
   }
   event->ignore();
}
