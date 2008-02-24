/**
 * PlayerFSMReady.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 */

#include <QSlider>

#include "PlayerFSMReady.hpp"
#include "PlayerWidget.hpp"
#include "ControlWidget.hpp"


PlayerFSMReady::PlayerFSMReady( PlayerWidget *playerWidget )
: PlayerFSMBase( playerWidget )
{
}


bool PlayerFSMReady::enter()
{
   mpPlayerWidget->mpStatusDisplay->setText( QWidget::tr("ready") );
   mpPlayerWidget->updateTime();
   mpPlayerWidget->mpControlWidget->allowInteractive( true );

   if( mpPlayerWidget->mAutoPlay )
   {
      mpPlayerWidget->mAutoPlay = false;
      mpPlayerWidget->mpFSM->changeState( PlayerFSM::playing );
   }

   if( mpPlayerWidget->mStartOther )
   {
      mpPlayerWidget->mStartOther = false;
      mpPlayerWidget->mpControlWidget->changeOtherState( mpPlayerWidget->mPlayer, PlayerFSM::searching );
   }
   
   return true;
}


void PlayerFSMReady::leave()
{
}


enum PlayerFSM::eState PlayerFSMReady::handleDerMixD( const QString &/*msg*/ )
{
   return PlayerFSM::ready;
}

