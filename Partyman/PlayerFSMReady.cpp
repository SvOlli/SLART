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
   if( mpPlayerWidget->mTotalTime < mpPlayerWidget->mHeadStart * 2 + 1 )
   {
      mpPlayerWidget->mpFSM->changeState( PlayerFSM::searching );
   }
   else
   {
      mpPlayerWidget->mpStatusDisplay->setText( QWidget::tr("ready") );
      mpPlayerWidget->mpStatusDisplay->setToolTip( QWidget::tr("right mouse button to unload") );
      mpPlayerWidget->updateTime();
      mpPlayerWidget->mpControlWidget->disableSkip( mpPlayerWidget->mAutoPlay );
      mpPlayerWidget->disablePlayPosition( false );

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
   }
   
   return true;
}


void PlayerFSMReady::leave()
{
   mpPlayerWidget->mpStatusDisplay->setToolTip( QString() );
}


enum PlayerFSM::eState PlayerFSMReady::handleDerMixD( const QString &/*msg*/ )
{
   return PlayerFSM::ready;
}

