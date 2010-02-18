/**
 * src/apps/Partyman/PlayerFSMReady.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
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
      if( mpPlayerWidget->mpControlWidget->isSkipChecked() )
      {
         mpPlayerWidget->mAutoPlay = true;
         mpPlayerWidget->mStartOther = true;
      }
      
      mpPlayerWidget->mpStatusDisplay->setText( QWidget::tr("ready") );
      mpPlayerWidget->mpStatusDisplay->setToolTip( QWidget::tr("right mouse button to unload") );
      mpPlayerWidget->updateTime();
      mpPlayerWidget->mpControlWidget->allowSkip();
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
      
      mpPlayerWidget->mpControlWidget->saveTracks( false );
      
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

