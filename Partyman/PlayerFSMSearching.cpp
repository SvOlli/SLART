/**
 * PlayerFSMSearching.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 */

#include "ControlWidget.hpp"
#include "PlayerFSMSearching.hpp"
#include "PlayerWidget.hpp"
#include "ScrollLine.hpp"


PlayerFSMSearching::PlayerFSMSearching( PlayerWidget *playerWidget )
: PlayerFSMBase( playerWidget )
{
}


bool PlayerFSMSearching::enter()
{
   mpPlayerWidget->mpStatusDisplay->setText( QWidget::tr("loading") );
   mpPlayerWidget->mTotalTime = 0;
   mpPlayerWidget->updateTime();
   mpPlayerWidget->sendCommand( "unwatch" );
   mpPlayerWidget->disablePlayPosition( true );
   
   mpPlayerWidget->getNextTrack();
   
   return true;
}


void PlayerFSMSearching::leave()
{
}


enum PlayerFSM::eState PlayerFSMSearching::handleDerMixD( const QString &msg )
{
   if( msg.startsWith( "[preread] success" ) )
   {
      mpPlayerWidget->sendCommand( "load", mpPlayerWidget->mpScrollLine->toolTip() );
   }
   
   if( msg.startsWith( "[preread] error" ) || msg.startsWith( "[load] error" ) )
   {
      mpPlayerWidget->getNextTrack();
   }
   
   if( msg.startsWith( "[load] success" ) )
   {
      return PlayerFSM::loading;
   }
   
   return PlayerFSM::searching;
}

