/**
 * src/apps/Partyman/PlayerFSMDisconnected.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "PlayerFSMDisconnected.hpp"

/* system headers */

/* Qt headers */
#include <QSlider>

/* local library headers */

/* local headers */
#include "PlayerWidget.hpp"
#include "TimeSlider.hpp"


PlayerFSMDisconnected::PlayerFSMDisconnected( PlayerWidget *playerWidget )
: PlayerFSMBase( playerWidget )
{
}


bool PlayerFSMDisconnected::enter()
{
   mpPlayerWidget->mpStatusDisplay->setText( QWidget::tr("unloaded") );
   mpPlayerWidget->getNextTrack( false );
   mpPlayerWidget->mTotalTime = 0;
   mpPlayerWidget->updateTime();
   mpPlayerWidget->mpPlayPosition->setDisabled( true );
   
   return true;
}


void PlayerFSMDisconnected::leave()
{
}


enum PlayerFSM::eState PlayerFSMDisconnected::handleDerMixD( const QString &/*msg*/ )
{
   return PlayerFSM::disconnected;
}

