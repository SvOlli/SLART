/**
 * PlayerFSMLoading.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 */

#include "PlayerFSMLoading.hpp"
#include "PlayerWidget.hpp"
#include "ScrollLine.hpp"
#include "Database.hpp"

#include "Trace.hpp"

PlayerFSMLoading::PlayerFSMLoading( PlayerWidget *playerWidget )
: PlayerFSMBase( playerWidget )
{
}


bool PlayerFSMLoading::enter()
{
   mpPlayerWidget->mpStatusDisplay->setText( QWidget::tr("loading") );
   mpPlayerWidget->mTotalTime = 0;
   mpPlayerWidget->updateTime();

   mInScan = false;
   
   mpPlayerWidget->mpDatabase->getTrackInfo( &(mpPlayerWidget->mTrackInfo), mpPlayerWidget->mpScrollLine->toolTip() );
   if( mpPlayerWidget->setVolume() )
   {
      mpPlayerWidget->sendCommand( "scan", "format length" );
   }
   else
   {
      /* request normalization volume */
      switch( mpPlayerWidget->mNormalizeMode )
      {
         case 0:
            mpPlayerWidget->sendCommand( "scan", "format length" );
            break;
         case 1:
            mpPlayerWidget->sendCommand( "scan", "format length peak" );
            break;
         case 2:
            mpPlayerWidget->sendCommand( "scan", "format length power" );
            break;
         default:
            break;
      }
   }
   
   return true;
}


void PlayerFSMLoading::leave()
{
}


enum PlayerFSM::eState PlayerFSMLoading::handleDerMixD( const QString &msg )
{
   if( mInScan )
   {
      mpPlayerWidget->handleScan( msg );
   }
   
   if( msg.startsWith( "[scan]" ) )
   {
      if( msg.endsWith( "+begin" ) )
      {
         mInScan = true;
      }
      if( msg.endsWith( "-end" ) )
      {
         mInScan = false;
         
         if( mpPlayerWidget->mTotalTime < (2 * mpPlayerWidget->mHeadStart) + 1 )
         {
            return PlayerFSM::searching;
         }
         else
         {
            return PlayerFSM::ready;
         }
      }
   }
   
   return PlayerFSM::loading;
}

