/**
 * PlayerFSMDisconnected.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 */

#include <QSlider>

#include "PlayerFSMDisconnected.hpp"
#include "PlayerWidget.hpp"

#include "Trace.hpp"


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

