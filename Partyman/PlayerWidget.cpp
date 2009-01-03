/**
 * PlayerWidget.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 */

#include "PlayerWidget.hpp"
#include "ControlWidget.hpp"
#include "Database.hpp"
#include "MySettings.hpp"
#include "ConfigDialog.hpp"

#include <iostream>
#include <QtGui>
#include <QTcpSocket>

#include "Trace.hpp"
#include "ScrollLine.hpp"
#include "PlayerFSM.hpp"


class MySlider : public QSlider
{
public:
   MySlider( Qt::Orientation orientation, QWidget *parent = 0 ) :
   QSlider( orientation, parent )
   {
   };
protected:
   void keyPressEvent( QKeyEvent *event )
   {
      QSlider::keyPressEvent( event );
      if( !event->isAutoRepeat() )
      {
         emit sliderPressed();
      }
   };
   void keyReleaseEvent( QKeyEvent *event )
   {
      QSlider::keyReleaseEvent( event );
      if( !event->isAutoRepeat() )
      {
         emit sliderReleased();
      }
   };
};


PlayerWidget::PlayerWidget( int index, Database *database,
                            ControlWidget *controlWidget, Qt::WindowFlags flags )
: QWidget( controlWidget, flags )
, mPlayer( index )
, mpDatabase( database )
, mpControlWidget( controlWidget )
, mpScrollLine( new ScrollLine( this ) )
, mpStatusDisplay( new QLabel( this ) )
, mpTimeDisplay( new QLabel( this ) )
, mpPlayPosition( new MySlider( Qt::Horizontal, this ) )
, mpSocket( new QTcpSocket( this ) )
, mpFSM( new PlayerFSM( this ) )
, mStartOther( false )
, mAutoPlay( false )
, mTotalTime( 0 )
, mFrequency( 44100 )
, mSamples( 0 )
, mHeadStart( 10 )
, mUpdateSlider( true )
, mDisplayPattern()
, mTrackInfo()
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
   
   connect( mpSocket, SIGNAL(connected()),
            this, SLOT(handleConnect()) );
   connect( mpSocket, SIGNAL(disconnected()),
            this, SLOT(handleDisconnect()) );
   connect( mpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(handleError(QAbstractSocket::SocketError)) );
   connect( mpSocket, SIGNAL(readyRead()),
            this, SLOT(handleResponse()) );
   
   connect( mpPlayPosition, SIGNAL(sliderPressed()),
            this, SLOT(lock()) );
   connect( mpPlayPosition, SIGNAL(sliderReleased()),
            this, SLOT(seek()) );
   connect( mpPlayPosition, SIGNAL(actionTriggered(int)),
            this, SLOT(playPosChange(int)) );
   connect( mpStatusDisplay, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(unload()) );
   
   mpFSM->changeState( PlayerFSM::disconnected );
}


PlayerWidget::~PlayerWidget()
{
   if( mpFSM ) delete mpFSM;
}


void PlayerWidget::getNextTrack( bool armed )
{
   QString fileName;
   bool inDatabase;
   
   if( armed )
   {
      mpControlWidget->getNextTrack( &fileName );
   }
   
   inDatabase = mpDatabase->getTrackInfo( &mTrackInfo, fileName );
   if( fileName.isEmpty() )
   {
      mpScrollLine->setText( tr("no file loaded") );
   }
   else
   {
      if( inDatabase && !mTrackInfo.mArtist.isEmpty() && !mTrackInfo.mTitle.isEmpty() )
      {
         mpScrollLine->setText( mTrackInfo.displayString( mDisplayPattern ) );
      }
      else
      {
         int lastSlash = fileName.lastIndexOf( '/' );
         int lastDot   = fileName.lastIndexOf( '.' );
         mpScrollLine->setText( fileName.mid( lastSlash+1, lastDot-lastSlash-1 ) );
      }
   }
   mpScrollLine->setToolTip( fileName );
   sendCommand( "preread", fileName );
}


QString PlayerWidget::getCurrentTrack() const
{
   return mpScrollLine->toolTip();
}


void PlayerWidget::readConfig()
{
   MySettings settings;
   mConsole        = settings.VALUE_DERMIXDLOG;
   mHeadStart      = settings.VALUE_CROSSFADETIME;
   mNormalizeMode  = settings.VALUE_NORMALIZEMODE;
   mNormalizeValue = settings.VALUE_NORMALIZEVALUE;
   mDisplayPattern = settings.VALUE_PLAYERPATTERN;
}


void PlayerWidget::lock()
{
   mUpdateSlider = false;
}


void PlayerWidget::seek()
{
   if( mPlayPosMoved )
   {
      sendCommand( "seek", QString::number(mpPlayPosition->sliderPosition()) );
   }
   mPlayPosMoved = false;
   mUpdateSlider = true;
}


void PlayerWidget::playPosChange( int /*action*/ )
{
   mPlayPosMoved = true;
   if( mpPlayPosition->sliderPosition() >= mTotalTime - 2 * mHeadStart )
   {
      mpPlayPosition->setSliderPosition( mTotalTime - 2 * mHeadStart );
   }
   QString fakemsg( QString::number( mpPlayPosition->sliderPosition() ) );
   fakemsg.append( "s," );
   updateTime( fakemsg, true );
}


void PlayerWidget::unload()
{
   if( mpFSM->getState() == PlayerFSM::ready )
   {
      QString actual( mpScrollLine->toolTip() );
      mpFSM->changeState( PlayerFSM::searching );
      mpControlWidget->addToPlaylist( QStringList( actual ) );
   }
}


void PlayerWidget::updateTime( const QString &msg, bool force )
{
   int colon = msg.indexOf(',');
   long playPosition = mpPlayPosition->value();

   if( mUpdateSlider || force )
   {
      if( msg.isEmpty() )
      {
         mpTimeDisplay->setText( TrackInfo::sec2minsec( 0 ) + "/" +
                                 TrackInfo::sec2minsec( mTotalTime ) );
         playPosition = 0;
      }
      else
      {
         if( (colon > 0) && (msg.at(colon-1) == 's') )
         {
            mpTimeDisplay->setText( TrackInfo::sec2minsec( msg.left(colon-1).toInt() ) + "/" +
                                    TrackInfo::sec2minsec( mTotalTime ) );
            playPosition = msg.left(colon-1).toLong();
         }
      }
   }
   
   if( mUpdateSlider )
   {
      mpPlayPosition->setValue( playPosition );
   }
   
   if( (mpFSM->getState() == PlayerFSM::playing) &&
       (playPosition >= mTotalTime - mHeadStart) )
   {
      mpFSM->changeState( PlayerFSM::ending );
   }
}


void PlayerWidget::setState( PlayerFSM::tState state )
{
   switch( mpFSM->getState() )
   {
      case PlayerFSM::searching:
      case PlayerFSM::loading:
         if( state == PlayerFSM::playing )
         {
            mAutoPlay = true;
            return;
         }
         /* slip through */
      default:
         break;
   }
   mpFSM->changeState( state );
}


void PlayerWidget::skip()
{
   switch( mpFSM->getState() )
   {
      case PlayerFSM::playing:
      case PlayerFSM::paused:
         if( MySettings().VALUE_COUNTSKIP )
         {
            ++mTrackInfo.mTimesPlayed;
            mpDatabase->updateTrackInfo( &mTrackInfo );
         }
         setState( PlayerFSM::searching );
         break;
      case PlayerFSM::searching:
      case PlayerFSM::loading:
      case PlayerFSM::ready:
         setState( PlayerFSM::playing );
         break;
      default:
         break;
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
      
      mpFSM->handleDerMixD( data );
      
      if( data.startsWith( "[connect]" ) )
      {
         /* check for version */
         int vpos = data.lastIndexOf(" v", -1, Qt::CaseInsensitive);
         int dotpos = data.indexOf(".",vpos);
         
         long  major = data.mid(vpos+2,dotpos-vpos-2).toLong();
         float minor = data.mid(dotpos+1).toFloat();
         bool  ok = false;
         if( (major == 1) && (minor > 4.999) )
         {
            ok = true;
         }
         
         if(!ok)
         {
            mpControlWidget->initDisconnect( ControlWidget::wrongVersion );
         }
      }
      
      if( data.startsWith( "[seek] success" ) )
      {
         updateTime( QString::number(mpPlayPosition->value()) + "s," );
      }
   }
}


void PlayerWidget::connectTo( const QString &hostname, int port )
{
   mpSocket->connectToHost( hostname, port );
}


void PlayerWidget::disconnect()
{
   sendCommand( "stop" );
   mpSocket->disconnectFromHost();
   setState( PlayerFSM::disconnected );
}


void PlayerWidget::handleConnect()
{
   if( mPlayer == 0 )
   {
      mStartOther = true;
      mAutoPlay   = true;
      setState( PlayerFSM::searching );
   }
   else
   {
      mStartOther = false;
      mAutoPlay   = false;
   }
}


void PlayerWidget::handleDisconnect()
{
   setState( PlayerFSM::disconnected );
}


void PlayerWidget::handleError( QAbstractSocket::SocketError /*socketError*/ )
{
   if( mpFSM->getState() != PlayerFSM::disconnected )
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
      if( command == "start" )
      {
         emit trackPlaying( mTrackInfo );
      }
      if( command != "preread" )
      {
         cmd.append( ' ' );
         cmd.append( '0' + mPlayer );
      }
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
   switch( mpFSM->getState() )
   {
      case PlayerFSM::playing:
         mpFSM->changeState( PlayerFSM::paused );
         break;
      case PlayerFSM::paused:
         mpFSM->changeState( PlayerFSM::playing );
         break;
      case PlayerFSM::ending:
         mpFSM->changeState( PlayerFSM::endingpaused );
         break;
      case PlayerFSM::endingpaused:
         mpFSM->changeState( PlayerFSM::ending );
         break;
      default:
         break;
   }
}


void PlayerWidget::handleScan( const QString &data )
{
   QStringList token( data.split(" ", QString::SkipEmptyParts) );
   bool ok;
   double max = token.at(0).toDouble( &ok );
   if( !ok )
   {
      return;
   }

   if( token.at(1).startsWith( "level" ) )
   {
      mTrackInfo.mVolume = max;
      mTrackInfo.setFlag( TrackInfo::ScannedWithPower, true );
      setVolume();
      mTrackInfo.mLastScanned = QDateTime::currentDateTime().toTime_t();
      mpDatabase->updateTrackInfo( &mTrackInfo );
   }
   else if( token.at(1).startsWith( "peak" ) )
   {
      mTrackInfo.mVolume = max;
      mTrackInfo.setFlag( TrackInfo::ScannedWithPeak, true );
      setVolume();
      mTrackInfo.mLastScanned = QDateTime::currentDateTime().toTime_t();
      mpDatabase->updateTrackInfo( &mTrackInfo );
   }
   else if( token.at(1).startsWith( "Hz" ) )
   {
      mFrequency = token.at(0).toLong();
      if( !mFrequency )
      {
         /* to avoid division by zero */
         mFrequency = 44100;
      }
   }
   else if( token.at(1).startsWith( "samples" ) )
   {
      mSamples = token.at(0).toLong();
   }
   mTotalTime = mSamples / mFrequency;
   
   if( (unsigned int)mTotalTime != mTrackInfo.mPlayTime )
   {
      mTrackInfo.mPlayTime = mTotalTime;
      mpDatabase->updateTrackInfo( &mTrackInfo );
   }
   mpPlayPosition->setRange( 0, mTotalTime );
   updateTime();
}


bool PlayerWidget::setVolume()
{
   double adjust = 1.0;
   
   if( mTrackInfo.mPlayTime > 0 )
   {
      mTotalTime = mTrackInfo.mPlayTime;
   }
   
   if( mTrackInfo.mVolume > 0.0 )
   {
      if( mTrackInfo.isFlagged( TrackInfo::ScannedWithPower ) )
      {
         adjust = mNormalizeValue / mTrackInfo.mVolume;
         sendCommand( "volume", QString::number(adjust) );
         return true;
      }
      if( mTrackInfo.isFlagged( TrackInfo::ScannedWithPeak ) )
      {
         adjust = 1.0 / mTrackInfo.mVolume;
         sendCommand( "volume", QString::number(adjust) );
         return true;
      }
   }
   return false;
}


void PlayerWidget::disablePlayPosition( bool disable )
{
   mpPlayPosition->setDisabled( disable );
}
