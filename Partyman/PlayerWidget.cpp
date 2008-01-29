/**
 * PlayerWidget.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 */

#include "PlayerWidget.hpp"
#include "ControlWidget.hpp"
#include "MySettings.hpp"

#include <iostream>
#include <QtGui>
#include <QTcpSocket>

#include "Trace.hpp"
#include "ScrollLine.hpp"

#if 0
static const char *strState[] = { "disconnected", "unloaded", "loading", "loaded", 
                                  "playing", "ending", "stopped", "seeking", "skipping",
                                  "paused", "pausedending" };
#endif

PlayerWidget::PlayerWidget( int index, 
                            ControlWidget *controlWidget, Qt::WindowFlags flags )
: QWidget( controlWidget, flags )
, mPlayer( index )
, mAutoStart( false )
, mShortPlay( false )
, mTotalTime( 0 )
, mHeadStart( 10 )
, mPlayState( disconnected )
, mUpdateSlider( true )
, mpControlWidget( controlWidget )
, mpScrollLine( new ScrollLine( this ) )
, mpStatusDisplay( new QLabel( this ) )
, mpTimeDisplay( new QLabel( this ) )
, mpPlayPosition( new QSlider( Qt::Horizontal, this ) )
, mpSocket( new QTcpSocket( this ) )
{
   QVBoxLayout *mainLayout = new QVBoxLayout( this );
   
#if QT_VERSION < 0x040300
   mainLayout->setMargin( 0 );
#else
   mainLayout->setContentsMargins( 0, 0, 0, 0 );
#endif
   mainLayout->setSpacing( 5 );
   mainLayout->addWidget( mpScrollLine );
   mainLayout->addWidget( mpTimeDisplay );
   mainLayout->addWidget( mpStatusDisplay );
   mainLayout->addWidget( mpPlayPosition );
   setLayout( mainLayout );
   
   mpStatusDisplay->setAlignment( Qt::AlignLeft );
   mpStatusDisplay->setFrameShape( QFrame::Box );
   mpStatusDisplay->setContextMenuPolicy( Qt::CustomContextMenu );
   mpTimeDisplay->setAlignment( Qt::AlignRight );
   mpTimeDisplay->setFrameShape( QFrame::Box );
   setInfo("");
   setPlayingStatus("");
   
   connect( mpSocket, SIGNAL(connected()), this, SLOT(handleConnect()) );
   connect( mpSocket, SIGNAL(disconnected()), this, SLOT(handleDisconnect()) );
   connect( mpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(handleError(QAbstractSocket::SocketError)) );
   connect( mpSocket, SIGNAL(readyRead()), this, SLOT(handleResponse()) );
   
   connect( mpPlayPosition, SIGNAL(sliderPressed()), this, SLOT(lock()) );
   connect( mpPlayPosition, SIGNAL(sliderReleased()), this, SLOT(seek()) );
   connect( mpStatusDisplay, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(unload(QPoint)) );
}


void PlayerWidget::readConfig()
{
   MySettings settings;
   mConsole        = settings.value( "DerMixDlog", false ).toBool();
   mHeadStart      = settings.value( "CrossfadeTime", 10 ).toInt();
   mNormalizeMode  = settings.value( "NormalizeMode",  0 ).toInt();
   mNormalizeValue = settings.value( "NormalizeValue", 0.4 ).toDouble();
}


void PlayerWidget::lock()
{
   mUpdateSlider = false;
}


void PlayerWidget::seek()
{
   sendCommand( "seek", QString::number(mpPlayPosition->value()) );
}


void PlayerWidget::unload( const QPoint &/*pos*/ )
{
   if( mPlayState == loaded )
   {
      QString actual( mpScrollLine->toolTip() );
      /* a bit of a hack: simulate track already played to get new one */
      mpControlWidget->reportState( mPlayer, stopped );
      mpControlWidget->addToPlaylist( QStringList( actual ) );
   }
}


QString PlayerWidget::sec2minsec( const QString &seconds )
{
   bool ok;
   long secs = seconds.toLong( &ok );
   long mins = secs / 60;
   secs %= 60;
   QString time( QString::number( mins ) );
   if( secs < 10 )
   {
      time.append( ":0" );
   }
   else
   {
      time.append( ":" );
   }
   time.append( QString::number( secs ) );

   return time;
}


void PlayerWidget::setInfo( const QString &line )
{
   int indexFile = line.indexOf( "file:" );
   int indexTime = line.indexOf( "/" ) + 1;
   int indexDot  = line.indexOf( ".", indexTime );
   int indexSec  = line.indexOf( "s", indexTime );
   int lengthTime;
   
   /* set filename */
   if( indexFile > 0 )
   {
      int lastSlash = line.lastIndexOf( '/' );
      int lastDot   = line.lastIndexOf( '.' );
      mpScrollLine->setText( line.mid( lastSlash+1, lastDot-lastSlash-1 ) );
      mpScrollLine->setToolTip( line.mid( indexFile + 6 ) );
   }
   else
   {
      setState( mPlayState == disconnected ? disconnected : unloaded );
   }

   if( line.indexOf( "loading" ) >= 0 )
   {
      setState( loading );
   }
   
   /* calculate total time */
   if( (indexDot > 0) && (indexDot < indexSec) )
   {
      lengthTime = indexDot - indexTime;
   }
   else
   {
      lengthTime = indexSec - indexTime;
   }
   mTotalTime = line.mid( indexTime, lengthTime ).toLong();
   mpPlayPosition->setRange( 0, mTotalTime );
   
   /* if total time is not sane decode track */
   if( mTotalTime == 1 )
   {
      sendCommand( "length" );
      sendCommand( "fullstat" );
      return;
   }
   
   /* if not playing set slider to start */
   if( line.indexOf( "playing" ) < 0 )
   {
      mpTimeDisplay->setText( sec2minsec( "0" ) + "/" + sec2minsec( QString::number( mTotalTime ) ) );
      mpPlayPosition->setValue( 0 );
   }
   
   /* if playing a short track */
   if( mShortPlay && line.indexOf( "paused" ) > 0 )
   {
      setState( playing );
   }
   
   /* test if playing a short track */
   if( mTotalTime > 0 )
   {
      mShortPlay = mTotalTime <= mHeadStart;
   }
   
   if( (mPlayState == loading) && (mNormalizeMode == 0) )
   {
      setState( loaded );
   }
}


void PlayerWidget::setPlayingStatus( const QString &line )
{
   int colon;
   colon = line.indexOf(',');
   long playPosition = mpPlayPosition->value();

   if( (colon > 0) && (line.at(colon-1) == 's') )
   {
      mpTimeDisplay->setText( sec2minsec( line.left(colon-1) ) + "/" + sec2minsec( QString::number( mTotalTime ) ) );
      playPosition = line.left(colon-1).toLong();
      if( mUpdateSlider )
      {
         mpPlayPosition->setValue( playPosition );
      }
   }
   
   if( (mPlayState == playing) && (mTotalTime > 0) && !mShortPlay &&
        ((mTotalTime - playPosition) <= mHeadStart) )
   {
      setState( ending );
   }
   
   if( line.indexOf( "stopped" ) > 0 )
   {
      if( mPlayState == loading )
      {
         setState( loaded );
      }
      else
      {
         setState( stopped );
      }
   }
}


void PlayerWidget::setState( eState state, const QString &parameter )
{
#if 0
TRACESTART(PlayerWidget::setState)
TRACEMSG << mPlayer << strState[mPlayState] << "->" << strState[state];
#endif
   switch( state )
   {
      case disconnected:
      case unloaded:
         mpStatusDisplay->setText( tr("unloaded") );
         mpScrollLine->setText( "no file loaded" );
         mpScrollLine->setToolTip( QString() );
         mTotalTime = 0;
         break;
      case loading:
         mpStatusDisplay->setText( tr("loading") );
         sendCommand( "load", parameter );
         mpPlayPosition->setDisabled( true );
         break;
      case loaded:
         mpStatusDisplay->setText( tr("ready") );
         mUpdateSlider = true;
         /* a bit of an ugly way to get things running */
         if( mAutoStart )
         {
            mpControlWidget->reportState( mPlayer, loaded );
            state = playing;
            mAutoStart = false;
         }
         else
         {
            break;
         }
      case playing:
         mpPlayPosition->setDisabled( false );
         sendCommand( "start" );
         break;
      case ending:
         mpPlayPosition->setDisabled( true );
         mpStatusDisplay->setText( tr("ending") );
         break;
      case stopped:
         mpStatusDisplay->setText( tr("stopped") );
         watch( false );
         break;
      case skipping:
         sendCommand( "stop" );
         break;
      case paused:
      case endingpaused:
         break;
      default:
         break;
   }

   mPlayState = state;
   mpControlWidget->reportState( mPlayer, mPlayState );
}


void PlayerWidget::skip()
{
   if( (mPlayState == playing) || (mPlayState == paused) )
   {
      setState( skipping );
   }
}


void PlayerWidget::handleResponse()
{
   while( mpSocket->bytesAvailable() )
   {
      QString data( QString::fromLocal8Bit( mpSocket->readLine() ) );
      data = data.simplified();
      if( !data.size() )
      {
         continue;
      }
      
      if( mConsole )
      {
         std::cout << mPlayer << '<' << data.toLocal8Bit().data() << '\n';
      }

      if( data.startsWith( "[ch" ) /*&& data.at(4) = ']'*/ ) /* from "[ch0]" */
      {
         setPlayingStatus( data.mid( 6 ) );
      }
      
      switch( mDerMixD )
      {
         case inFullstat:
            if( data.startsWith( QString("in ")+QChar('0'+mPlayer) ) ) /* from "in 0:" */
            {
               setInfo( data.mid( 5 ) );
            }
            break;
         case inScan:
            adjustVolume( data );
            break;
         case inNormal:
            break;
         default:
            break;
      }
      
      if( data.startsWith( "[scan]" ) )
      {
         if( data.endsWith( "+begin" ) )
         {
            mDerMixD = inScan;
         }
         if( data.endsWith( "-end" ) )
         {
            mDerMixD = inNormal;
         }
      }
      
      if( data.startsWith( "[fullstat]" ) )
      {
         if( data.endsWith( "+begin" ) )
         {
            mDerMixD = inFullstat;
         }
         if( data.endsWith( "-end" ) )
         {
            mDerMixD = inNormal;
         }
      }
      
      if( data.startsWith( "[load] error" ))
      {
         mpControlWidget->loadNext( mPlayer );
      }
      
      if( data.startsWith( "[load] success" ) )
      {
         sendCommand( "fullstat" );
         
         /* request normalization volume */
         switch( mNormalizeMode )
         {
            case 1:
               sendCommand( "scan", "peak" );
               break;
            case 2:
               sendCommand( "scan", "power" );
               break;
            default:
               break;
         }
      }
      
      if( data.startsWith( "[start] success" ) )
      {
         watch( true );
         if( mPlayState == ending )
         {
            mpStatusDisplay->setText( tr("ending") );
         }
         else
         {
            mpStatusDisplay->setText( tr("playing") );
         }
      }
      
      if( data.startsWith( "[pause] success" ) )
      {
         mpStatusDisplay->setText( tr("paused") );
      }
      
      if( data.startsWith( "[seek] success" ) )
      {
         mUpdateSlider = true;
      }
      
      if( data.startsWith( "[connect]" ) )
      {
         setState( unloaded );
         
         /* check for version */
         int vpos = data.lastIndexOf(" v", -1, Qt::CaseInsensitive);
         int dotpos = data.indexOf(".",vpos);
         
         long  major = data.mid(vpos+2,dotpos-vpos-2).toLong();
         float minor = data.mid(dotpos+1).toFloat();
         bool  ok = false;
         if( (major == 1) && (minor > 2.999) )
         {
            ok = true;
         }
         
         if(!ok)
         {
            mpControlWidget->initDisconnect( ControlWidget::wrongVersion );
         }
      }
   }
}


void PlayerWidget::watch( bool turnWatchOn )
{
   if( mWatching )
   {
      if( !turnWatchOn )
      {
         sendCommand( "unwatch" );
      }
   }
   else
   {
      if( turnWatchOn )
      {
         sendCommand( "watch" );
      }
   }
   mWatching = turnWatchOn;
}


void PlayerWidget::connectTo( const QString &hostname, int port )
{
   mpSocket->connectToHost( hostname, port );
}


void PlayerWidget::disconnect()
{
   sendCommand( "stop" );
   mpSocket->disconnectFromHost();
   setState( disconnected );
}


void PlayerWidget::handleConnect()
{
   mAutoStart = (mPlayer == 0);
   mWatching = false;
   mDerMixD = inNormal;
}


void PlayerWidget::handleDisconnect()
{
   setState( disconnected );
   setInfo("");
}


void PlayerWidget::handleError( QAbstractSocket::SocketError /*socketError*/ )
{
   if( mPlayState != disconnected )
   {
      mpControlWidget->initDisconnect( ControlWidget::connectionLost );
   }
   else
   {
      mpControlWidget->initDisconnect( ControlWidget::noConnection );
   }
}


void PlayerWidget::sendCommand( const QString &command, const QString &parameter )
{
   if( mpSocket->state() == QAbstractSocket::ConnectedState )
   {
      QString cmd( command );
      cmd.append( ' ' );
      cmd.append( '0' + mPlayer );
      if( !parameter.isEmpty() )
      {
         cmd.append( ' ' );
         cmd.append( parameter );
      }
      cmd.append( '\n' );
      
      QByteArray block( cmd.toLocal8Bit() );
      if( mConsole )
      {
         std::cout << mPlayer << '>' << block.data();
      }
      
      mpSocket->write( block );
   }
}


void PlayerWidget::pause()
{
   if( mPlayState == playing )
   {
      mPlayState = paused;
      sendCommand( "pause" );
      mpControlWidget->reportState( mPlayer, mPlayState );
      return;
   }
   
   if( mPlayState == ending )
   {
      mPlayState = endingpaused;
      sendCommand( "pause" );
      return;
   }
   
   if( mPlayState == paused )
   {
      mPlayState = playing;
      sendCommand( "start" );
      mpControlWidget->reportState( mPlayer, mPlayState );
      return;
   }
   
   if( mPlayState == endingpaused )
   {
      mPlayState = ending;
      sendCommand( "start" );
      return;
   }
}


void PlayerWidget::adjustVolume( const QString &data )
{
   QStringList token( data.split(" ") );
   bool ok;
   double adjust = 1.0;
   double max = token.at(0).toDouble( &ok );
   if( !ok )
   {
      return;
   }

   if( token.at(1).startsWith( "level" ) )
   {
      adjust = mNormalizeValue / max;
   }
   else if( token.at(1).startsWith( "peak" ) )
   {
      adjust = 1.0 / max;
   }
   sendCommand( "volume", QString::number(adjust) );
   if( mPlayState != playing )
   {
      setState( loaded );
   }
}


QString PlayerWidget::getFileName()
{
   return mpScrollLine->toolTip();
}
