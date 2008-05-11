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


PlayerWidget::PlayerWidget( int index, 
                            ControlWidget *controlWidget, Qt::WindowFlags flags )
: QWidget( controlWidget, flags )
, mPlayer( index )
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
   
   if( armed )
   {
      mpControlWidget->getNextTrack( &fileName );
   }
   if( fileName.isEmpty() )
   {
      mpScrollLine->setText( tr("no file loaded") );
   }
   else
   {
      int lastSlash = fileName.lastIndexOf( '/' );
      int lastDot   = fileName.lastIndexOf( '.' );
      mpScrollLine->setText( fileName.mid( lastSlash+1, lastDot-lastSlash-1 ) );
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
   updateTime( fakemsg );
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


void PlayerWidget::updateTime( const QString &msg )
{
   int colon = msg.indexOf(',');
   long playPosition = mpPlayPosition->value();

   if( msg.isEmpty() )
   {
      mpTimeDisplay->setText( sec2minsec( "0" ) + "/" + sec2minsec( QString::number( mTotalTime ) ) );
      playPosition = 0;
   }
   else
   {
      if( (colon > 0) && (msg.at(colon-1) == 's') )
      {
         mpTimeDisplay->setText( sec2minsec( msg.left(colon-1) ) + "/" + sec2minsec( QString::number( mTotalTime ) ) );
         playPosition = msg.left(colon-1).toLong();
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
      if(command != "preread")
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
      sendCommand( "volume", QString::number(adjust) );
   }
   else if( token.at(1).startsWith( "peak" ) )
   {
      adjust = 1.0 / max;
      sendCommand( "volume", QString::number(adjust) );
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
   
   mpPlayPosition->setRange( 0, mTotalTime );
   updateTime();
}


void PlayerWidget::disablePlayPosition( bool disable )
{
   mpPlayPosition->setDisabled( disable );
}
