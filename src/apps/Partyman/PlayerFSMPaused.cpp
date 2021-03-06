/*
 * src/apps/Partyman/PlayerFSMPaused.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "PlayerFSMPaused.hpp"

/* system headers */

/* Qt headers */

/* local library headers */

/* local headers */
#include "ControlWidget.hpp"
#include "PlayerWidget.hpp"


PlayerFSMPaused::PlayerFSMPaused( PlayerWidget *playerWidget )
: PlayerFSMBase( playerWidget )
{
}


bool PlayerFSMPaused::enter()
{
   mpPlayerWidget->mpStatusDisplay->setText( QWidget::tr("paused") );
   mpPlayerWidget->sendCommand( "pause" );
   mpPlayerWidget->mpControlWidget->log( "p0a", "pause" );

   return true;
}


void PlayerFSMPaused::leave()
{
}


enum PlayerFSM::eState PlayerFSMPaused::handleDerMixD( const QString &/*msg*/ )
{
   return PlayerFSM::paused;
}

