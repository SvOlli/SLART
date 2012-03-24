/*
 * src/apps/Partyman/PlayerFSMLoading.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "PlayerFSMLoading.hpp"

/* system headers */

/* Qt headers */
#include <QDateTime>
#include <QFileInfo>

/* local library headers */
#include <Database.hpp>
#include <ScrollLine.hpp>

/* local headers */
#include "PlayerWidget.hpp"
#include "TimeSlider.hpp"


PlayerFSMLoading::PlayerFSMLoading( PlayerWidget *playerWidget )
: PlayerFSMBase( playerWidget )
{
}


bool PlayerFSMLoading::enter()
{
   bool needRescan = false;
   mpPlayerWidget->mpStatusDisplay->setText( QWidget::tr("loading") );
   mpPlayerWidget->mTotalTime = 0;
   mpPlayerWidget->updateTime();

   mInScan = false;
   QString fileName( mpPlayerWidget->mpScrollLine->toolTip() );

#if 0
   mpPlayerWidget->mpDatabase->getTrackInfo( &(mpPlayerWidget->mTrackInfo), fileName );
#endif
   if( mpPlayerWidget->setVolume() )
   {
      QFileInfo qfi( fileName );
      if( qfi.lastModified().toTime_t() > mpPlayerWidget->mTrackInfo.mLastScanned )
      {
         needRescan = true;
      }
      else
      {
         mpPlayerWidget->mpPlayPosition->setRange( 0, mpPlayerWidget->mTrackInfo.mPlayTime );
      }
   }
   else
   {
      needRescan = true;
   }

   if( needRescan )
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
   else
   {
      mpPlayerWidget->mpFSM->changeState( PlayerFSM::ready );
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
      else if( msg.endsWith( "-end" ) )
      {
         mInScan = false;

         return PlayerFSM::ready;
      }
      else if( msg.startsWith( "[scan] error:" ) )
      {
         mpPlayerWidget->mpFSM->changeState( PlayerFSM::searching );
      }
   }

   return PlayerFSM::loading;
}

