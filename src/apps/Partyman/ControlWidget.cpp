/**
 * src/apps/Partyman/ControlWidget.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "ControlWidget.hpp"

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
#include "PlaylistControlWidget.hpp"


ControlWidget::ControlWidget( Database *database, ConfigDialog *config,
                              PlaylistControlWidget *playlist, QWidget *parent, Qt::WindowFlags flags )
: QWidget( parent, flags )
, mpConfig( config )
, mpPlaylist( playlist )
, mpSatellite( Satellite::get( this ) )
, mpGenericSatMsgHandler( new GenericSatMsgHandler( mpSatellite, GenericSatMsgHandler::WithPingAndDialog ) )
, mpSettingsButton( new QPushButton( tr("Settings"), this ) )
, mpConnectButton( new QPushButton( tr("Connect"), this ) )
, mpSkipButton( new QPushButton( tr("Next"), this ) )
, mConnected( false )
, mPaused( false )
, mPartymanIcon( QIcon(":/PartymanSmile.png") )
, mStopIcon( QIcon(":/Stop.png") )
, mPlayIcon( QIcon(":/Play.png") )
, mPauseIcon( QIcon(":/Pause.png") )
, mSkipIcon( QIcon(":/Skip.png") )
, mLoadIcon( QIcon(":/Load.png") )
, mpTrayIcon( new QSystemTrayIcon( this ) )
, mpTrayIconStopMenu( new QMenu( this ) )
, mpTrayIconPlayMenu( new QMenu( this ) )
, mpDisconnectMenu( new QMenu( mpConnectButton ) )
, mpPlayAction( mpTrayIconStopMenu->addAction( mPlayIcon, tr("Start" ) ) )
, mpSkipAction( mpTrayIconPlayMenu->addAction( mSkipIcon, tr("Next" ) ) )
, mpPauseAction( mpDisconnectMenu->addAction( tr("Pause" ) ) )
, mpDisconnectAction( mpDisconnectMenu->addAction( mStopIcon, tr("Disconnect" ) ) )
, mpLoadAction( mpDisconnectMenu->addAction( mLoadIcon, tr("Load" ) ) )
, mKioskMode( false )
, mTrayIconClickTimer( this )
, mDerMixDprocess()
, mLoggerProcess()
, mWaitForDerMixD( false )
, mDerMixDstarted( false )
, mLastP0p()
{
   MySettings settings;
   mpPlayer[0] = new PlayerWidget(0, database, this);
   mpPlayer[1] = new PlayerWidget(1, database, this);
   mTrayIconClickTimer.setSingleShot( true );
   
   QGridLayout *mainLayout    = new QGridLayout( this );
   
   mainLayout->setContentsMargins( 0, 0, 0, 0 );
   mainLayout->setSpacing( 5 );
   mainLayout->addWidget( mpPlayer[0],      0, 0, 4, 1 );
   mainLayout->addWidget( mpPlayer[1],      0, 2, 4, 1 );
   mainLayout->addWidget( mpSettingsButton, 0, 1 );
   mainLayout->addWidget( mpConnectButton,  2, 1 );
   mainLayout->addWidget( mpSkipButton,     3, 1 );
   
   mainLayout->setColumnStretch( 0, 1 );
   mainLayout->setColumnStretch( 2, 1 );
   mainLayout->setRowStretch( 1, 1 );
   mainLayout->setRowStretch( 2, 1 );
   
   setLayout( mainLayout );
   
   mpConnectButton->setCheckable( true );
   mpConnectButton->setDisabled( true );

   mpSkipButton->setCheckable( true );
   mpSkipButton->setDisabled( true );
   mpSkipAction->setDisabled( true );
   
   mpTrayIcon->setIcon( QIcon(":/PartymanSmile.png") );
   mpTrayIcon->setContextMenu( mpTrayIconStopMenu );
   mpTrayIconPlayMenu->addAction( mpPauseAction );
   mpTrayIconPlayMenu->addAction( mpDisconnectAction );
   
   connect( mpSettingsButton, SIGNAL(clicked()),
            mpConfig, SLOT(exec()) );
   connect( mpConnectButton, SIGNAL(clicked()),
            this, SLOT(initConnect()) );
   connect( mpPlayAction, SIGNAL(triggered()),
            this, SLOT(initConnect()) );
   connect( mpPauseAction, SIGNAL(triggered()),
            this, SLOT(handlePause()) );
   connect( mpDisconnectAction, SIGNAL(triggered()),
            this, SLOT(initDisconnect()) );
   connect( mpLoadAction, SIGNAL(triggered()),
            this, SLOT(handleLoad()) );
   connect( mpSkipButton, SIGNAL(clicked()),
            this, SLOT(handleSkipTrack()) );
   connect( mpSkipAction, SIGNAL(triggered()),
            this, SLOT(handleSkipTrack()) );
   connect( mpConfig, SIGNAL(configChanged()),
            this, SLOT(readConfig()) );
   connect( mpSatellite, SIGNAL(received(const QByteArray &)),
            this, SLOT(handleSatellite(const QByteArray &)) );
   connect( mpGenericSatMsgHandler, SIGNAL(updateConfig()),
            mpConfig, SLOT(readSettings()) );
   connect( mpGenericSatMsgHandler, SIGNAL(anotherInstance()),
            this, SLOT(initDisconnect()) );
   connect( mpPlayer[0], SIGNAL(trackPlaying(const TrackInfo &)),
            this, SLOT(handleTrackPlaying(const TrackInfo &)) );
   connect( mpPlayer[1], SIGNAL(trackPlaying(const TrackInfo &)),
            this, SLOT(handleTrackPlaying(const TrackInfo &)) );
   connect( mpTrayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(handleTrayIcon(QSystemTrayIcon::ActivationReason)) );
   connect( &mTrayIconClickTimer, SIGNAL(timeout()),
            this, SLOT(handlePause()) );

   connect( &mDerMixDprocess, SIGNAL(readyReadStandardError()),
            this, SLOT(handleDerMixDstartup()) );
   connect( &mDerMixDprocess, SIGNAL(finished(int,QProcess::ExitStatus)),
            this, SLOT(handleDerMixDfinish(int,QProcess::ExitStatus)) );
   connect( &mDerMixDprocess, SIGNAL(error(QProcess::ProcessError)),
            this, SLOT(handleDerMixDerror(QProcess::ProcessError)) );

#if 0
   QShortcut *skip = new QShortcut( QKeySequence(Qt::Key_F5), this );
   
   connect( skip, SIGNAL(activated()),
            this, SLOT(handleSkipTrack()) );
#endif
   
   mpSettingsButton->setObjectName( QString("SettingsButton") );
   
   setSizePolicy( QSizePolicy::Minimum, QSizePolicy::Fixed );
}


ControlWidget::~ControlWidget()
{
   saveTracks( false );
   mpPlayer[0]->disconnect();
   mpPlayer[1]->disconnect();
   
   if( mDerMixDprocess.state() != QProcess::NotRunning )
   {
      mDerMixDprocess.terminate();
      mDerMixDprocess.waitForFinished();
   }
   if( mDerMixDprocess.state() != QProcess::NotRunning )
   {
      mDerMixDprocess.kill();
      mDerMixDprocess.waitForFinished();
   }
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
   
   mpPlaylist->savePlaylist( current, next );
   
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
   }
}


void ControlWidget::readConfig()
{
   MySettings settings;
   mpSatellite->restart();
   mpPlayer[0]->readConfig();
   mpPlayer[1]->readConfig();
   if( settings.VALUE_DERMIXDRUN )
   {
      mpConnectButton->setText("Start");
      mpDisconnectAction->setText("Stop");
   }
   else
   {
      mpConnectButton->setText("Connect");
      mpDisconnectAction->setText("Disconnect");
   }
   mpTrayIcon->setVisible( settings.VALUE_TRAYICON && QSystemTrayIcon::isSystemTrayAvailable() );
   mpLoadAction->setEnabled( MySettings( "Global" ).VALUE_CLIPBOARDMODE > 0 );
}


void ControlWidget::addToPlaylist( const QStringList &entries )
{
   emit requestAddToPlaylist( entries, true );
}


void ControlWidget::initConnect()
{
   MySettings settings;
   
   if( !mConnected )
   {
      QString hostname( "localhost" );
      int port = settings.VALUE_DERMIXDPORT;
      
      if( settings.VALUE_DERMIXDRUN )
      {
         QMutex mutex;
         QStringList args;
         QString params( settings.VALUE_DERMIXDPARAMS );
         args << "-c" << "-p" << QString::number( port );
         if( !params.isEmpty() )
         {
            args << params.split(' ');
         }
         mDerMixDprocess.start( settings.VALUE_DERMIXDCMD, args );
         /* block until dermixd is up an running */
         for( mWaitForDerMixD = true; mWaitForDerMixD; )
         {
            QCoreApplication::processEvents();
         }
         if( !mDerMixDstarted )
         {
            QMessageBox::critical( this, QApplication::applicationName(),
                                   QString(tr("Could not start DerMixD")) );
         }
      }
      else
      {
         mDerMixDstarted = true;
         hostname = settings.VALUE_DERMIXDHOST;
      }
      mConnected = true;
      mpPlayer[0]->connectTo( hostname, port );
      mpPlayer[1]->connectTo( hostname, port );
      mpConnectButton->setMenu( mpDisconnectMenu );
      mpTrayIcon->setContextMenu( mpTrayIconPlayMenu );
      mpConnectButton->setChecked( true );
      mpSkipButton->setDisabled( mKioskMode );
      mpSkipAction->setDisabled( mKioskMode );
      emit signalConnected( true );
   }
   handlePause( true );
}


void ControlWidget::initDisconnect( eErrorCode errorcode )
{
   emit requestChangeTitle( mPartymanIcon, QApplication::applicationName()+tr(" (disconnected)") );
   mpTrayIcon->setToolTip( QString() );
   mpTrayIcon->setContextMenu( mpTrayIconStopMenu );
   if( mConnected )
   {
      mConnected = false;
      QString errorText;
      saveTracks( true );
      mpPlayer[0]->disconnect();
      mpPlayer[1]->disconnect();
      mpSkipButton->setDisabled( true );
      mpSkipAction->setDisabled( true );
      mpConnectButton->setMenu( 0 );
      mpConnectButton->setChecked( false );
      switch( errorcode )
      {
         case noConnection:
            errorText = tr("Could not connect to DerMixD. Is it running?");
            break;
         case connectionLost:
            errorText = tr("The connection to DerMixD was lost.");
            break;
         case wrongVersion:
            errorText = tr("The Version of DerMixD is not compatible.");
            break;
         case noError:
         default:
            break;
      }
      if( !errorText.isEmpty() && mDerMixDstarted )
      {
         QMessageBox::critical( this, QApplication::applicationName(), errorText );
      }
      if( MySettings().VALUE_DERMIXDRUN )
      {
         mDerMixDprocess.terminate();
      }
      emit signalConnected( false );
      mpPlaylist->getTrack( TrackInfo() );
      log( "p0s", "stop" );
   }
}


void ControlWidget::handlePause( bool reset )
{
   mpPlayer[0]->pause();
   mpPlayer[1]->pause();
   if( mPaused || reset )
   {
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
      emit requestChangeTitle( mPauseIcon, QApplication::applicationName()+tr(" (paused)") );
      mpPauseAction->setIcon( mPlayIcon );
      mpPauseAction->setText( tr("Resume") );
      if( mKioskMode )
      {
         mpPauseAction->setDisabled( false );
      }
      mPaused = true;
   }
   mpConnectButton->setMenu( mpDisconnectMenu );
}


void ControlWidget::handleLoad()
{
   mpPlaylist->addEntries( GlobalConfigWidget::getClipboard().split(QRegExp("(\r|\n)"),QString::SkipEmptyParts) );
}


void ControlWidget::handleSkipTrack()
{
   if( mpSkipButton->isEnabled() )
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
      
      mpSkipButton->setChecked( checkstate );
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
   mpConnectButton->setDisabled( !allowed );
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
      
      if( src.at(0) == "P0A" )
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
      
      if( src.at(0) == "P0N" )
      {
         handleSkipTrack();
      }

      if( src.at(0) == "P0S" )
      {
         if( !mKioskMode )
         {
            initDisconnect();
         }
      }

      if( src.at(0) == "P0R" )
      {
         if( !mLastP0p.isEmpty() )
         {
            mpSatellite->send( mLastP0p );
         }
      }

      if( src.at(0) == "P0C" )
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

      if( src.at(0) == "P0T" )
      {
         if( src.size() > 1 )
         {
            emit requestTab( src.at(1).toInt() );
         }
      }

      if( (src.at(0) == "k0u") ||
          (src.at(0) == "r0u") )
      {
         emit trackUpdate();
      }
   }
}


void ControlWidget::log( const QString &udpEvent, const QString &logEvent, const QString &data )
{
   MySettings settings;

   QByteArray msg( udpEvent.toUtf8() );
   if( !data.isEmpty() )
   {
      msg.append( '\n' );
      msg.append( data.toUtf8() );
   }
   mpSatellite->send( msg );
   if( udpEvent == "p0p" )
   {
      mLastP0p = msg;
   }
   if( udpEvent == "p0s" )
   {
      mLastP0p.clear();
   }
   
   QString command( settings.VALUE_LOGCMD );
   if( !command.isEmpty() )
   {
      QStringList args;
      args << logEvent;
      if( !data.isEmpty() )
      {
         args << data;
      }
      mLoggerProcess.start( command, args );
   }
}


void ControlWidget::handleDerMixDstartup()
{
   QStringList data( QString::fromLocal8Bit( mDerMixDprocess.readAllStandardError() ).split('\n') );

   for( int i = 0; i < data.size(); i++ )
   {
      if( data.at(i).indexOf( " online on port " ) > 0 )
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
   MySettings settings;
   /* pass through to track info widget */
   mpPlaylist->getTrack( trackInfo );
   QString title( trackInfo.displayString( settings.VALUE_NAMEPATTERN ) );
   QString bubble( trackInfo.displayString( settings.VALUE_TRAYICONPATTERN ) );
   if( trackInfo.mTitle.isEmpty() )
   {
      title  = QApplication::applicationName();
      bubble = trackInfo.mDirectory;
      bubble.append( '\n' );
      bubble.append( trackInfo.mFileName );
   }
   emit requestChangeTitle( mPlayIcon, title );
   mpTrayIcon->setToolTip( bubble );
   if( settings.VALUE_TRAYICON && 
       settings.VALUE_TRAYICONBUBBLE && 
       QSystemTrayIcon::supportsMessages() )
   {
      mpTrayIcon->showMessage( tr("Now Playing:"), bubble,
         (QSystemTrayIcon::MessageIcon)(settings.VALUE_TRAYICONBUBBLEICON),
         (int)(settings.VALUE_TRAYICONBUBBLETIME * 1000) );
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
            mTrayIconClickTimer.start( QApplication::doubleClickInterval() );
         }
      }
      else
      {
         initConnect();
      }
   }
   if( reason == QSystemTrayIcon::DoubleClick )
   {
      mTrayIconClickTimer.stop();
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
   mpDisconnectAction->setDisabled( mKioskMode );
   mpSkipButton->setDisabled( mKioskMode | !mConnected );
   if( mKioskMode )
   {
      mpSkipButton->setText( tr("Kiosk Mode") );
   }
   else
   {
      mpSkipButton->setText( tr("Next") );
   }
}


void ControlWidget::allowSkip()
{
   mpSkipButton->setChecked( false );
}
