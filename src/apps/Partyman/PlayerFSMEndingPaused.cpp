/**
 * src/apps/Partyman/PlayerFSMEndingPaused.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#include "PlayerFSMEndingPaused.hpp"
#include "PlayerWidget.hpp"


PlayerFSMEndingPaused::PlayerFSMEndingPaused( PlayerWidget *playerWidget )
: PlayerFSMBase( playerWidget )
{
}


bool PlayerFSMEndingPaused::enter()
{
   mpPlayerWidget->mpStatusDisplay->setText( QWidget::tr("paused") );
   mpPlayerWidget->sendCommand( "pause" );

   return true;
}


void PlayerFSMEndingPaused::leave()
{
   mpPlayerWidget->sendCommand( "start" );
}


enum PlayerFSM::eState PlayerFSMEndingPaused::handleDerMixD( const QString &/*msg*/ )
{
   return PlayerFSM::endingpaused;
}

