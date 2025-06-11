/*
 * src/apps/Partyman/PlayerWidget.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "PlayerWidget.hpp"

/* system headers */
#include <cmath>
#include <iostream>

/* Qt headers */
#include <QBoxLayout>
#include <QDateTime>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QLabel>
#include <QMimeData>
#include <QProcess>
#include <QTcpSocket>

/* local library headers */
#include <Database.hpp>
#include <Settings.hpp>
#include <ScrollLine.hpp>

/* local headers */
//#include "PartymanConfigDialog.hpp"
#include "ControlWidget.hpp"
#include "PlayerFSM.hpp"
#include "TimeSlider.hpp"


static inline double to_db( double factor )
{
   return factor > 2.5e-10 ? std::log(factor) / std::log(10) * 20 : -192;
}


PlayerWidget::PlayerWidget( int index, Database *database,
                            ControlWidget *controlWidget, Qt::WindowFlags flags )
: QWidget( controlWidget, flags )
, mPlayer( index )
, mpDatabase( database )
, mpControlWidget( controlWidget )
, mpScrollLine( new ScrollLine( this ) )
, mpStatusDisplay( new QLabel( this ) )
, mpTimeDisplay( new QLabel( this ) )
, mpPlayPosition( new TimeSlider( Qt::Horizontal, this ) )
, mpSocket( new QTcpSocket( this ) )
, mpFSM( new PlayerFSM( this ) )
, mpNotifier( new QProcess( this ) )
, mStartOther( false )
, mAutoPlay( false )
, mTotalTime( 0 )
, mEndTime( 0 )
, mFrequency( 44100 )
, mSamples( 0 )
, mHeadStart( 10 )
, mUpdateSlider( true )
, mKioskMode( false )
, mDisplayPattern()
, mTrackInfo()
{
   QVBoxLayout *mainLayout = new QVBoxLayout( this );

   mainLayout->setContentsMargins( 0, 0, 0, 0 );
   mainLayout->setSpacing( 5 );
   mainLayout->addWidget( mpScrollLine );
   mainLayout->addWidget( mpTimeDisplay );
   mainLayout->addWidget( mpStatusDisplay );
   mainLayout->addWidget( mpPlayPosition );
   setLayout( mainLayout );

   mpStatusDisplay->setAlignment( Qt::AlignLeft );
   mpStatusDisplay->setFrameShape( QFrame::Box );
   mpStatusDisplay->setContextMenuPolicy( Qt::CustomContextMenu );
   mpStatusDisplay->setObjectName( QString("StatusLabel") );
   mpTimeDisplay->setAlignment( Qt::AlignRight );
   mpTimeDisplay->setFrameShape( QFrame::Box );
   mpTimeDisplay->setObjectName( QString("TimeLabel") );
   mpPlayPosition->setTickPosition( QSlider::TicksBelow );
   mpPlayPosition->setTickInterval( 60 );
   mpPlayPosition->setMaximum( 1 );

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
   connect( mpNotifier, SIGNAL(finished(int,QProcess::ExitStatus)),
            this, SLOT(handleNotifierClose(int,QProcess::ExitStatus)) );

   mpFSM->changeState( PlayerFSM::disconnected );
   QProcessEnvironment env;
   env.insert( "PLAYER", QString::number( index ) );
   mpNotifier->setProcessEnvironment( env );
}


PlayerWidget::~PlayerWidget()
{
   if( mpFSM ) delete mpFSM;
}


PlayerFSM::tState PlayerWidget::getState()
{
   return mpFSM->getState();
}


void PlayerWidget::handleNotifierDone( int exitCode, QProcess::ExitStatus exitStatus )
{
   mpNotifier->close();
   if( exitStatus != QProcess::NormalExit )
   {
      qDebug() << mpNotifier->program() << "return with exit code:" << exitCode;
   }
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
   updateNotifier( mTrackInfo.toProcessEnvironment() );
   if( fileName.isEmpty() )
   {
      mpScrollLine->setText( tr("no file loaded") );
      mpScrollLine->setDragFileName( fileName );
   }
   else
   {
      if( inDatabase && !mTrackInfo.mArtist.isEmpty() && !mTrackInfo.mTitle.isEmpty() )
      {
         mpScrollLine->setText( mTrackInfo.displayString( mDisplayPattern ) );
         mpScrollLine->setDragInfo( mTrackInfo );
      }
      else
      {
         int lastSlash = fileName.lastIndexOf( '/' );
         int lastDot   = fileName.lastIndexOf( '.' );
         mpScrollLine->setText( fileName.mid( lastSlash+1, lastDot-lastSlash-1 ) );
         mTrackInfo.mDirectory = fileName.left( lastSlash+1 );
         mTrackInfo.mFileName  = fileName.mid( lastSlash+1 );
         mpScrollLine->setDragFileName( fileName );
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
   mConsole        = Settings::value( Settings::PartymanDerMixDlog );
   mHeadStart      = Settings::value( Settings::PartymanCrossfadeTime );
   mNormalizeMode  = Settings::value( Settings::PartymanNormalizeMode );
   mNormalizeValue = Settings::value( Settings::PartymanNormalizeValue );
   mDisplayPattern = Settings::value( Settings::PartymanPlayerPattern );
   QProcessEnvironment env( mpNotifier->processEnvironment() );
   env.insert( "DERMIXD_HOST", Settings::value( Settings::PartymanDerMixDhost ) );
   env.insert( "DERMIXD_PORT", QString::number( Settings::value( Settings::PartymanDerMixDport ) ) );
   env.insert( "CROSSFADE_TIME", QString::number( Settings::value( Settings::PartymanCrossfadeTime ) ) );
   env.insert( "USE_SATELLITE", QString::number( Settings::value( Settings::CommonUseSatellite ) ) );
   env.insert( "SATELLITE+PORT", QString::number( Settings::value( Settings::GlobalSatellitePort ) ) );
   env.insert( "LOG_CMD", Settings::value( Settings::PartymanLogCmd ) );
   mpNotifier->setProcessEnvironment( env );
   mpNotifier->setProgram( Settings::value( Settings::PartymanExecuteOnStatusChange ) );
   mpFSM->setNotifier( mpNotifier->program().isEmpty() ? 0 : mpNotifier );
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


void PlayerWidget::nextTrack()
{
   setState( PlayerFSM::searching );
}


void PlayerWidget::playPosChange( int /*action*/ )
{
   mPlayPosMoved = true;
   if( mpPlayPosition->sliderPosition() >= mTotalTime - 2 * mHeadStart - 1 )
   {
      mpPlayPosition->setSliderPosition( mTotalTime - 2 * mHeadStart - 1 );
   }
   QString fakemsg( QString::number( mpPlayPosition->sliderPosition() ) );
   fakemsg.append( "s," );
   updateTime( fakemsg, true );
}


void PlayerWidget::unload( bool drop )
{
   if( mpFSM->getState() == PlayerFSM::ready )
   {
      QString actual( mpScrollLine->toolTip() );
      mpFSM->changeState( PlayerFSM::searching );
      if( !drop )
      {
         mpControlWidget->addToPlaylist( QStringList( actual ) );
      }
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
            if( playPosition >= (mEndTime + mHeadStart) )
            {
               QTimer::singleShot( 1000, this, SLOT(nextTrack()) );
            }
         }
      }
   }

   if( mUpdateSlider )
   {
      mpPlayPosition->setValue( playPosition );
   }

   if( (mpFSM->getState() == PlayerFSM::playing) &&
       (playPosition >= mEndTime /*mTotalTime - mHeadStart*/ ) )
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


bool PlayerWidget::skip()
{
   PlayerFSM::tState currentState = mpFSM->getState();
   switch( currentState )
   {
      case PlayerFSM::playing:
      case PlayerFSM::paused:
         if( Settings::value( Settings::PartymanCountSkip ) )
         {
            ++(mTrackInfo.mTimesPlayed);
            mpDatabase->updateTrackInfo( &mTrackInfo );
         }
         if( Settings::value( Settings::PartymanCrossfadeOnNext ) )
         {
            mEndTime = mpPlayPosition->value() + mHeadStart;
            setState( PlayerFSM::ending );
         }
         else
         {
            setState( PlayerFSM::searching );
         }
         break;
      case PlayerFSM::ending:
      case PlayerFSM::searching:
      case PlayerFSM::loading:
         mAutoPlay = true;
         mStartOther = true;
         return false;
         break;
      case PlayerFSM::ready:
         setState( PlayerFSM::playing );
         break;
      default:
         break;
   }
   return true;
}


void PlayerWidget::handleResponse()
{
   while( mpSocket->bytesAvailable() )
   {
      QString data( QString::fromLocal8Bit( mpSocket->readLine().constData() ) );
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
         if( ((major == 1) && (minor > 4.999) ) || (major >= 2) )
         {
            ok = true;
         }

         if(!ok)
         {
            mpControlWidget->initDisconnect( ControlWidget::ErrorWrongVersion );
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
   mpPlayPosition->setMaximum( 1 );
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
      mpControlWidget->initDisconnect( ControlWidget::ErrorConnectionLost );
   }
   else
   {
      mpControlWidget->initDisconnect( ControlWidget::ErrorNoConnection );
   }
}


void PlayerWidget::sendCommand( const QString &command, const QString &parameter )
{
   if( mpSocket->state() == QAbstractSocket::ConnectedState )
   {
      QString cmd( command );
      if( command == "start" )
      {
         if( mpFSM->getState() == PlayerFSM::playing )
         {
            mpDatabase->getTrackInfo( &mTrackInfo );
            emit trackPlaying( mTrackInfo );
         }
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
         mpFSM->changeState( PlayerFSM::playing );
         break;
      default:
         break;
   }
}


void PlayerWidget::handleScan( const QString &data )
{
   QStringList token( data.split(" ", Qt::SkipEmptyParts) );
   bool ok;
   double max = token.at(1).toDouble( &ok );
   if( !ok )
   {
      return;
   }

   if( token.at(0).startsWith( "power:" ) )
   {
      mTrackInfo.mVolume = max;
      mTrackInfo.setFlag( TrackInfo::ScannedWithPower, true );
      setVolume();
      mTrackInfo.mLastScanned = QDateTime::currentDateTime().toTime_t();
      mpDatabase->updateTrackInfo( &mTrackInfo );
   }
   else if( token.at(0).startsWith( "peak:" ) )
   {
      mTrackInfo.mVolume = max;
      mTrackInfo.setFlag( TrackInfo::ScannedWithPeak, true );
      setVolume();
      mTrackInfo.mLastScanned = QDateTime::currentDateTime().toTime_t();
      mpDatabase->updateTrackInfo( &mTrackInfo );
   }
   else if( token.at(0).startsWith( "format:" ) )
   {
      mFrequency = token.at(1).toLong();
      if( !mFrequency )
      {
         /* to avoid division by zero */
         mFrequency = 44100;
      }
   }
   else if( token.at(0).startsWith( "length:" ) )
   {
      mSamples = token.at(1).toLong();
   }
   mTotalTime = mSamples / mFrequency;
   mEndTime = mTotalTime - mHeadStart;

   if( (unsigned int)mTotalTime != mTrackInfo.mPlayTime )
   {
      mTrackInfo.mPlayTime = mTotalTime;
      mpDatabase->updateTrackInfo( &mTrackInfo );
   }
   mpPlayPosition->setMaximum( mTotalTime );
   updateTime();
}


bool PlayerWidget::setVolume()
{
   double adjust = 1.0;

   if( mTrackInfo.mPlayTime > 0 )
   {
      mTotalTime = mTrackInfo.mPlayTime;
      mEndTime = mTotalTime - mHeadStart;
   }

   if( mTrackInfo.mVolume > 0.0 )
   {
      if( mTrackInfo.isFlagged( TrackInfo::ScannedWithPower ) )
      {
         adjust = mNormalizeValue / mTrackInfo.mVolume;
         sendCommand( "preamp", QString::number(to_db(adjust)) );
         return true;
      }
      if( mTrackInfo.isFlagged( TrackInfo::ScannedWithPeak ) )
      {
         adjust = 1.0 / mTrackInfo.mVolume;
         sendCommand( "preamp", QString::number(to_db(adjust)) );
         return true;
      }
   }
   return false;
}


void PlayerWidget::disablePlayPosition( bool disable )
{
   mpPlayPosition->setDisabled( disable );
}


void PlayerWidget::handleKioskMode( bool enable )
{
   mKioskMode = enable;
   if( mpFSM->getState() != PlayerFSM::ready )
   {
      mpPlayPosition->setDisabled( mKioskMode );
   }
}


void PlayerWidget::dragEnterEvent( QDragEnterEvent *event )
{
   if( (event->source() != mpScrollLine) && (mpFSM->getState() == PlayerFSM::ready) )
   {
      const QMimeData *mimeData = event->mimeData();
      if( mimeData->hasUrls() )
      {
         event->acceptProposedAction();
         return;
      }
   }
   event->ignore();
}


void PlayerWidget::dropEvent( QDropEvent *event )
{
   if( mpFSM->getState() == PlayerFSM::ready )
   {
      const QMimeData *mimeData = event->mimeData();
      if( mimeData->hasUrls() )
      {
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
         fileNames << mpScrollLine->toolTip();
         mpControlWidget->addToPlaylist( fileNames );
         mpFSM->changeState( PlayerFSM::searching );
         event->acceptProposedAction();
         return;
      }
   }
   event->ignore();
}


void PlayerWidget::updateNotifier( const QString &name, const QString &value )
{
   QProcessEnvironment env( mpNotifier->processEnvironment() );
   env.insert( name, value );
   mpNotifier->setProcessEnvironment( env );
}


void PlayerWidget::updateNotifier( const QProcessEnvironment &environment )
{
   QProcessEnvironment env( mpNotifier->processEnvironment() );
   env.insert( environment );
   mpNotifier->setProcessEnvironment( env );
}

