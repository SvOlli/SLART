/**
 * src/apps/Partyman/PlayerFSMEnding.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "PlayerFSMEnding.hpp"

/* system headers */

/* Qt headers */

/* local library headers */
#include <Database.hpp>
#include <TrackInfo.hpp>

/* local headers */
#include "ControlWidget.hpp"
#include "PlayerWidget.hpp"


PlayerFSMEnding::PlayerFSMEnding( PlayerWidget *playerWidget )
: PlayerFSMBase( playerWidget )
{
}


bool PlayerFSMEnding::enter()
{
   mpPlayerWidget->mpStatusDisplay->setText( QWidget::tr("ending") );
   mpPlayerWidget->mpControlWidget->changeOtherState( mpPlayerWidget->mPlayer, PlayerFSM::playing );
   mpPlayerWidget->disablePlayPosition( true );

   if( mpPlayerWidget->mTrackInfo.mID > 0 )
   {
      mpPlayerWidget->mpDatabase->getTrackInfo( &(mpPlayerWidget->mTrackInfo) );
      ++(mpPlayerWidget->mTrackInfo.mTimesPlayed);
      mpPlayerWidget->mpDatabase->disableNotify();
      mpPlayerWidget->mpDatabase->updateTrackInfo( &(mpPlayerWidget->mTrackInfo) );
   }

   return true;
}


void PlayerFSMEnding::leave()
{
}


enum PlayerFSM::eState PlayerFSMEnding::handleDerMixD( const QString &msg )
{
   if( msg.startsWith( QString("[ch")+QChar('0'+mpPlayerWidget->mPlayer)+']' ) )
   {
      mpPlayerWidget->updateTime( msg.mid(6) );

      if( msg.endsWith( "stopped" ) )
      {
         return PlayerFSM::searching;
      }
   }

   return PlayerFSM::ending;
}

