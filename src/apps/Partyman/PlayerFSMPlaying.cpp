/*
 * src/apps/Partyman/PlayerFSMPlaying.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "PlayerFSMPlaying.hpp"

/* system headers */

/* Qt headers */
#include <QSlider>

/* local library headers */
#include <ScrollLine.hpp>

/* local headers */
#include "ControlWidget.hpp"
#include "PlayerWidget.hpp"
#include "TimeSlider.hpp"


PlayerFSMPlaying::PlayerFSMPlaying( PlayerWidget *playerWidget )
: PlayerFSMBase( playerWidget )
{
}


bool PlayerFSMPlaying::enter()
{
   mpPlayerWidget->mpStatusDisplay->setText( QWidget::tr("playing") );
   mpPlayerWidget->sendCommand( "start" );
// ThOr: Why seeking again here, already seeked on moving the slider?
//   mpPlayerWidget->sendCommand( "seek", QString::number(mpPlayerWidget->mpPlayPosition->value()) );
   mpPlayerWidget->mpControlWidget->log( "p0p", "play", mpPlayerWidget->mpScrollLine->toolTip() );
   mpPlayerWidget->sendCommand( "watch" );
   mpPlayerWidget->disablePlayPosition( mpPlayerWidget->mKioskMode );

   return true;
}


void PlayerFSMPlaying::leave()
{
}


enum PlayerFSM::eState PlayerFSMPlaying::handleDerMixD( const QString &msg )
{
   if( msg.startsWith( QString("[ch")+QChar('0'+mpPlayerWidget->mPlayer)+']' ) )
   {
      mpPlayerWidget->updateTime( msg.mid(6) );
   }

   return PlayerFSM::playing;
}

