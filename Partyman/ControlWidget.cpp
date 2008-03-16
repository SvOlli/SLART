/**
 * ControlWidget.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 */

#include "ControlWidget.hpp"
#include "PlaylistWidget.hpp"
#include "ConfigDialog.hpp"
#include "MySettings.hpp"

#include <QtGui>

#include "Trace.hpp"


ControlWidget::ControlWidget( ConfigDialog *config, PlaylistWidget *playlist, QWidget *parent, Qt::WindowFlags flags )
: QWidget( parent, flags )
, mpConfig( config )
, mpPlaylist( playlist )
, mpLogo( new QLabel( this ) )
, mpConnectButton( new QPushButton( tr("Connect"), this ) )
, mpSkipButton( new QPushButton( tr("Next"), this ) )
, mConnected( false )
, mPaused( false )
, mStopIcon( QIcon(":/Stop.gif") )
, mPlayIcon( QIcon(":/Play.gif") )
, mPauseIcon( QIcon(":/Pause.gif") )
, mpDisconnectMenu( new QMenu( mpConnectButton ) )
, mpPauseAction( mpDisconnectMenu->addAction( tr("Pause" ) ) )
, mpDisconnectAction( mpDisconnectMenu->addAction( mStopIcon, tr("Disconnect" ) ) )
, mSLARTCom( this )
, mDerMixDprocess()
, mLoggerProcess()
, mWaitForDerMixD( false )
, mDerMixDstarted( false )
{
   MySettings settings;
   mpPlayer[0] = new PlayerWidget(0, this);
   mpPlayer[1] = new PlayerWidget(1, this);
   
   QHBoxLayout *mainLayout    = new QHBoxLayout( this );
   QVBoxLayout *centralLayout = new QVBoxLayout( );
   
#if QT_VERSION < 0x040300
   mainLayout->setMargin( 0 );
#else
   mainLayout->setContentsMargins( 0, 0, 0, 0 );
#endif
   mainLayout->setSpacing( 5 );
   centralLayout->addWidget( mpLogo );
   centralLayout->addWidget( mpConnectButton );
   centralLayout->addWidget( mpSkipButton );
   mainLayout->addWidget( mpPlayer[0] );
   mainLayout->addLayout( centralLayout );
   mainLayout->addWidget( mpPlayer[1] );
   
   setLayout( mainLayout );
   
   mpLogo->setText( QApplication::applicationName() );
   mpLogo->setAlignment( Qt::AlignCenter );
   mpLogo->setFrameShadow( QFrame::Raised );
   mpLogo->setFrameShape( QFrame::Box );
   
   mpConnectButton->setCheckable( true );
   mpConnectButton->setDisabled( true );

   mpSkipButton->setDisabled( true );
   
   connect( mpConnectButton, SIGNAL(clicked()), this, SLOT(initConnect()) );
   connect( mpPauseAction, SIGNAL(triggered()), this, SLOT(handlePause()) );
   connect( mpDisconnectAction, SIGNAL(triggered()), this, SLOT(initDisconnect()) );
   connect( mpSkipButton, SIGNAL(clicked()), this, SLOT(handleSkipTrack()) );
   connect( mpConfig, SIGNAL(configChanged()), this, SLOT(readConfig()) );
   connect( &mSLARTCom, SIGNAL(packageRead(QStringList)), this, SLOT(handleSLART(QStringList)) );

   connect( &mDerMixDprocess, SIGNAL(readyReadStandardError()),
            this, SLOT(handleDerMixDstartup()) );
   connect( &mDerMixDprocess, SIGNAL(finished(int,QProcess::ExitStatus)),
            this, SLOT(handleDerMixDfinish(int,QProcess::ExitStatus)) );
   connect( &mDerMixDprocess, SIGNAL(error(QProcess::ProcessError)),
            this, SLOT(handleDerMixDerror(QProcess::ProcessError)) );

   connect( this, SIGNAL(signalConnected(bool)),
            mpConfig, SLOT(handleConnected(bool)) );

   setSizePolicy( QSizePolicy::Minimum, QSizePolicy::Fixed );
}


ControlWidget::~ControlWidget()
{
   saveTracks();
   mpPlayer[0]->disconnect();
   mpPlayer[1]->disconnect();
   
   /* yeah, let's celebrate brute force */
   if( mDerMixDprocess.state() != QProcess::NotRunning )
   {
      mDerMixDprocess.terminate();
   }
   while( mDerMixDprocess.state() != QProcess::NotRunning )
   {
      mDerMixDprocess.kill();
      QCoreApplication::processEvents();
   }
}


void ControlWidget::saveTracks()
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
   
   if( !next.isEmpty() )
   {
      mpPlaylist->addEntries( QStringList( next ), true );
   }
   if( !current.isEmpty() )
   {
      mpPlaylist->addEntries( QStringList( current ), true );
   }
}


void ControlWidget::readConfig()
{
   MySettings settings;
   mSLARTCom.resetReceiver();
   mpPlayer[0]->readConfig();
   mpPlayer[1]->readConfig();
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
      int port = settings.value( "DerMixDport", 8888 ).toInt();
      
      if( settings.value( "DerMixDrun", false ).toBool() )
      {
         QMutex mutex;
         QStringList args;
         QString params( settings.value( "DerMixDparams", "" ).toString() );
         args << "-c" << "-p" << QString::number( port );
         if( !params.isEmpty() )
         {
            args << params.split(' ');
         }
         /* TODO: configure path to dermixd */
         mDerMixDprocess.start( settings.value("DerMixDcmd", "dermixd").toString(), args );
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
         hostname = settings.value( "DerMixDhost", QString("localhost") ).toString();
      }
      mConnected = true;
      mpPlayer[0]->connectTo( hostname, port );
      mpPlayer[1]->connectTo( hostname, port );
      mpConnectButton->setMenu( mpDisconnectMenu );
      mpConnectButton->setChecked( true );
      emit signalConnected( true );
   }
   handlePause( true );
}


void ControlWidget::initDisconnect( eErrorCode errorcode )
{
   emit requestChangeTitle( mStopIcon, QApplication::applicationName()+tr(" (disconnected)") );
   if( mConnected )
   {
      mConnected = false;
      QString errorText;
      saveTracks();
      mpPlayer[0]->disconnect();
      mpPlayer[1]->disconnect();
      mpSkipButton->setDisabled( true );
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
      if( MySettings().value( "DerMixDrun", false ).toBool() )
      {
         mDerMixDprocess.terminate();
      }
      emit signalConnected( false );
      log( "p0s", "stop" );
   }
}


void ControlWidget::handlePause( bool reset )
{
   mpPlayer[0]->pause();
   mpPlayer[1]->pause();
   if( mPaused || reset )
   {
      emit requestChangeTitle( mPlayIcon, QApplication::applicationName() );
      mpPauseAction->setIcon( mPauseIcon );
      mpPauseAction->setText( tr( "Pause" ) );
      mPaused = false;
   }
   else
   {
      emit requestChangeTitle( mPauseIcon, QApplication::applicationName()+tr(" (paused)") );
      mpPauseAction->setIcon( mPlayIcon );
      mpPauseAction->setText( tr( "Resume" ) );
      mPaused = true;
   }
   if( (mpPlayer[0]->getState() != PlayerFSM::ending) &&
       (mpPlayer[1]->getState() != PlayerFSM::ending) )
   {
      mpSkipButton->setDisabled( mPaused );
   }
   mpConnectButton->setMenu( mpDisconnectMenu );
}


void ControlWidget::handleSkipTrack()
{
   mpSkipButton->clearFocus();
   allowInteractive( false );
   log( "p0n", "skip" );
   mpPlayer[0]->skip();
   mpPlayer[1]->skip();
}


void ControlWidget::getNextTrack( QString *fileName )
{
   mpPlaylist->getNextTrack( fileName );
}


void ControlWidget::allowConnect( bool allowed )
{
   mpConnectButton->setDisabled( !allowed );
}


void ControlWidget::handleSLART( const QStringList &src )
{
   if( (src.at(0) == "P0Q") || (src.at(0) == "s0d") || (src.at(0) == "f0d") )
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
         handlePause();
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
      initDisconnect();
   }
}


void ControlWidget::log( const QString &udpEvent, const QString &logEvent, const QString &data )
{
   MySettings settings;

   QString udp( udpEvent );
   if( !data.isEmpty() )
   {
      udp.append( '\n' );
      udp.append( data );
   }
   settings.sendNotification( udp );
   
   QString command( settings.value( "LogCmd", "" ).toString() );
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
      if( data.at(i).startsWith( "[port_watcher] online" ) )
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


void ControlWidget::allowInteractive( bool allow )
{
   mpSkipButton->setDisabled( !allow );
   mpPlayer[0]->disablePlayPosition( !allow );
   mpPlayer[1]->disablePlayPosition( !allow );
}
