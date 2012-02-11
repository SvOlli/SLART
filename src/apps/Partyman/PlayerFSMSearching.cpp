/*
 * src/apps/Partyman/PlayerFSMSearching.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "PlayerFSMSearching.hpp"

/* system headers */

/* Qt headers */
#include <QSlider>

/* local library headers */
#include <ScrollLine.hpp>

/* local headers */
#include "ControlWidget.hpp"
#include "PlayerWidget.hpp"
#include "TimeSlider.hpp"


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
   mpPlayerWidget->mpPlayPosition->setMaximum( 1 );

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

