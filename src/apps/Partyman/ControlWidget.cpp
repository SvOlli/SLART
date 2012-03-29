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

/* Qt headers */
#include <QtGui>

/* local library headers */
#include <GenericSatMsgHandler.hpp>
#include <GlobalConfigWidget.hpp>
#include <Satellite.hpp>
#include <Settings.hpp>
#include <WidgetShot.hpp>

/* local headers */
#include "ConfigDialog.hpp"
#include "PartymanMainWindow.hpp"


ControlWidget::ControlWidget( Database *database, ConfigDialog *config,
                              PartymanMainWindow *parent )
: QWidget( parent )
, mpConfig( config )
, mpPlaylist( parent )
, mpSatellite( Satellite::get() )
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
   setAcceptDrops( true );
   mpSettingsButton->setObjectName( QString("SettingsButton") );
   mpPlayer[0] = new PlayerWidget(0, database, this);
   mpPlayer[1] = new PlayerWidget(1, database, this);
   mTrayIconClickTimer.setSingleShot( true );

   QGridLayout *mainLayout    = new QGridLayout( this );

   mainLayout->setContentsMargins( 3, 3, 3 ,3 );
   mainLayout->setSpacing( 5 );
   mainLayout->addWidget( mpPlayer[0],      1, 0, 4, 1 );
   mainLayout->addWidget( mpPlayer[1],      1, 2, 4, 1 );
   mainLayout->addWidget( mpSettingsButton, 1, 1 );
   mainLayout->addWidget( mpConnectButton,  3, 1 );
   mainLayout->addWidget( mpSkipButton,     4, 1 );

   mainLayout->setColumnStretch( 0, 1 );
   mainLayout->setColumnStretch( 2, 1 );
   mainLayout->setRowStretch( 0, 1 );
   mainLayout->setRowStretch( 5, 1 );

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
   connect( mpSatellite, SIGNAL(received(QByteArray)),
            this, SLOT(handleSatellite(QByteArray)) );
   connect( mpGenericSatMsgHandler, SIGNAL(updateConfig()),
            mpConfig, SLOT(readSettings()) );
   connect( mpGenericSatMsgHandler, SIGNAL(anotherInstance()),
            this, SLOT(initDisconnect()) );
   connect( mpPlayer[0], SIGNAL(trackPlaying(TrackInfo)),
            this, SLOT(handleTrackPlaying(TrackInfo)) );
   connect( mpPlayer[1], SIGNAL(trackPlaying(TrackInfo)),
            this, SLOT(handleTrackPlaying(TrackInfo)) );
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
}


ControlWidget::~ControlWidget()
{
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


void ControlWidget::readConfig()
{
   mpSatellite->restart();
   mpPlayer[0]->readConfig();
   mpPlayer[1]->readConfig();
   if( Settings::value( Settings::PartymanDerMixDrun ) )
   {
      mpConnectButton->setText("Start");
      mpDisconnectAction->setText("Stop");
   }
   else
   {
      mpConnectButton->setText("Connect");
      mpDisconnectAction->setText("Disconnect");
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
         args << "-c" << "-p" << QString::number( port );
         if( !params.isEmpty() )
         {
            args << params.split(' ');
         }
         mDerMixDprocess.start( Settings::value( Settings::PartymanDerMixDcmd ), args );
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
      }
      else
      {
         mDerMixDstarted = true;
         hostname = Settings::value( Settings::PartymanDerMixDhost );
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


void ControlWidget::initDisconnect( ErrorCode errorCode )
{
   emit requestChangeTitle( mPartymanIcon, QApplication::applicationName() +
                            ": " + tr("(disconnected)") );
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
         mDerMixDprocess.terminate();
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
      emit requestChangeTitle( mPauseIcon, QApplication::applicationName() +
                               ": " + tr("(paused)") );
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

   QString command( Settings::value( Settings::PartymanLogCmd ) );
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
   QStringList data( QString::fromLocal8Bit( mDerMixDprocess.readAllStandardError().constData() ).split('\n') );

   foreach( const QString &line, data )
   {
      if( line.indexOf( " online on port " ) > 0 )
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
   emit requestChangeTitle( mPlayIcon, title );
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
      if( (state[0] == PlayerFSM::ready) )
      {
         player = 0;
      }
      else if( (state[1] == PlayerFSM::ready) )
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
         QMetaObject::invokeMethod( mpConnectButton, "animateClick",
                                    Qt::QueuedConnection,
                                    Q_ARG( int, 500 ) );
      }
      event->acceptProposedAction();
      return;
   }
   event->ignore();
}
