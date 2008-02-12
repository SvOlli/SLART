/**
 * ControlWidget.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 */

#include "ControlWidget.hpp"
#include "ConfigDialog.hpp"
#include "MySettings.hpp"

#include <QtGui>

#include "Trace.hpp"


ControlWidget::ControlWidget( ConfigDialog *config, QWidget *parent , Qt::WindowFlags flags )
: QWidget( parent, flags )
, mpConfig( config )
, mpLogo( new QLabel( this ) )
, mpConnectButton( new QPushButton( this ) )
, mpSkipButton( new QPushButton( this ) )
, mConnected( false )
, mPaused( false )
, mStopIcon( QIcon(":/Stop.gif") )
, mPlayIcon( QIcon(":/Play.gif") )
, mPauseIcon( QIcon(":/Pause.gif") )
, mpDisconnectMenu( new QMenu( mpConnectButton ) )
, mpPauseAction( mpDisconnectMenu->addAction( tr("Pause" ) ) )
, mpDisconnectAction( mpDisconnectMenu->addAction( mStopIcon, tr("Disconnect" ) ) )
, mSLATCom( this )
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
   
   mpConnectButton->setText( tr("Connect") );
   mpConnectButton->setCheckable( true );
   mpConnectButton->setDisabled( true );

   mpSkipButton->setText( tr("Next") );
   mpSkipButton->setDisabled( true );
   
   connect( mpConnectButton, SIGNAL(clicked()), this, SLOT(initConnect()) );
   connect( mpPauseAction, SIGNAL(triggered()), this, SLOT(handlePause()) );
   connect( mpDisconnectAction, SIGNAL(triggered()), this, SLOT(initDisconnect()) );
   connect( mpSkipButton, SIGNAL(clicked()), this, SLOT(nextTrack()) );
   connect( mpConfig, SIGNAL(configChanged()), this, SLOT(readConfig()) );
   connect( &mSLATCom, SIGNAL(packageRead(QStringList)), this, SLOT(handleSLAT(QStringList)) );

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


void ControlWidget::readConfig()
{
   MySettings settings;
   mSLATCom.resetReceiver();
   mpPlayer[0]->readConfig();
   mpPlayer[1]->readConfig();
}


void ControlWidget::addToPlaylist( const QStringList &entries )
{
   emit requestAddToPlaylist( entries, true );
}


void ControlWidget::initConnect()
{
#if 0
TRACESTART(ControlWidget::initConnect)
#endif
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
#if 0
TRACEMSG << "dermixd" << args;
#endif
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
#if 0
TRACEMSG << "started";
#endif
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
   if( (mpPlayer[0]->getState() != PlayerWidget::ending) &&
       (mpPlayer[1]->getState() != PlayerWidget::ending) )
   {
      mpSkipButton->setDisabled( mPaused );
   }
   mpConnectButton->setMenu( mpDisconnectMenu );
}


void ControlWidget::nextTrack()
{
   mpSkipButton->clearFocus();
   mpSkipButton->setDisabled( true );
   mpPlayer[0]->skip();
   mpPlayer[1]->skip();
   log( "p0n", "skip" );
}


void ControlWidget::loadNext( int player )
{
   mLastLoad = player;
   emit requestNextTrack( player );
}


void ControlWidget::setFileName( int player, const QString &fileName )
{
   mpPlayer[player]->setState( PlayerWidget::loading, fileName );
}


void ControlWidget::reportState( int player, PlayerWidget::eState state )
{
   if( state == PlayerWidget::playing )
   {
      log( "p0p", "play", mpPlayer[player]->getFileName() );
   }

   if( state == PlayerWidget::paused )
   {
      log( "p0a", "pause" );
   }

   if( state == PlayerWidget::ending )
   {
      mpSkipButton->setDisabled( true );
      mpPlayer[1-player]->setState( PlayerWidget::playing );
   }
   
   if( state == PlayerWidget::unloaded )
   {
      /* evil hack for quick startup part 1 */
      if( player == 0 )
      {
         loadNext( player );
      }
   }
   
   if( state == PlayerWidget::loading )
   {
      mpSkipButton->setDisabled( true );
   }
   
   if( state == PlayerWidget::loaded )
   {
      mpSkipButton->setDisabled( false );
      /* evil hack for quick startup part 2 */
      if( (player == 0) && 
          (mpPlayer[1]->getState() == PlayerWidget::unloaded) )
      {
         loadNext(1);
      }
   }
   
   if( state == PlayerWidget::stopped )
   {
      loadNext( player );
      switch( mpPlayer[1-player]->getState() )
      {
         case PlayerWidget::ending:
            mpPlayer[player]->setState( PlayerWidget::playing );
            break;
         case PlayerWidget::loaded:
            mpPlayer[1-player]->setState( PlayerWidget::playing );
            break;
         default:
            break;
      }
   }
}


void ControlWidget::allowConnect( bool allowed )
{
   mpConnectButton->setDisabled( !allowed );
}


void ControlWidget::handleSLAT( const QStringList &src )
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
#if 0
TRACESTART(ControlWidget::handleDerMixDstartup)
#endif
   QStringList data( QString::fromLocal8Bit( mDerMixDprocess.readAllStandardError() ).split('\n') );
#if 0
TRACEMSG << data;
#endif

   for( int i = 0; i < data.size(); i++ )
   {
      if( data.at(i).startsWith( "[port_watcher] online" ) )
      {
#if 0
TRACEMSG << "unlock";
#endif
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
