/**
 * src/apps/Partyman/PlayerFSM.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "PlayerFSM.hpp"

/* system headers */

/* Qt headers */

/* local library headers */

/* local headers */
#include "ControlWidget.hpp"
#include "PlayerFSMDisconnected.hpp"
#include "PlayerFSMSearching.hpp"
#include "PlayerFSMLoading.hpp"
#include "PlayerFSMReady.hpp"
#include "PlayerFSMEnding.hpp"
#include "PlayerFSMPlaying.hpp"
#include "PlayerFSMPaused.hpp"
#include "PlayerFSMEndingPaused.hpp"
#include "PlayerWidget.hpp"


PlayerFSM::PlayerFSM( PlayerWidget *playerWidget )
: mState( ready )
, mpStateDisconnected( new PlayerFSMDisconnected(playerWidget) )
, mpStateSearching( new PlayerFSMSearching(playerWidget) )
, mpStateLoading( new PlayerFSMLoading(playerWidget) )
, mpStateReady( new PlayerFSMReady(playerWidget) )
, mpStatePlaying( new PlayerFSMPlaying(playerWidget) )
, mpStateEnding( new PlayerFSMEnding(playerWidget) )
, mpStatePaused( new PlayerFSMPaused(playerWidget) )
, mpStateEndingPaused( new PlayerFSMEndingPaused(playerWidget) )
, mpStateClass( 0 )
{
}


PlayerFSM::~PlayerFSM()
{
   if( mpStateDisconnected ) delete mpStateDisconnected;
   if( mpStateSearching )    delete mpStateSearching;
   if( mpStateLoading )      delete mpStateLoading;
   if( mpStateReady )        delete mpStateReady;
   if( mpStatePlaying )      delete mpStatePlaying;
   if( mpStateEnding )       delete mpStateEnding;
   if( mpStatePaused )       delete mpStatePaused;
   if( mpStateEndingPaused ) delete mpStateEndingPaused;
}


bool PlayerFSM::changeState( enum eState newState )
{
   if( newState == mState )
   {
      return true;
   }

   if( mpStateClass )
   {
      mpStateClass->leave();
   }

   mState = newState;
   switch( newState )
   {
      case disconnected:
         mpStateClass = mpStateDisconnected;
         break;
      case searching:
         mpStateClass = mpStateSearching;
         break;
      case loading:
         mpStateClass = mpStateLoading;
         break;
      case ready:
         mpStateClass = mpStateReady;
         break;
      case playing:
         mpStateClass = mpStatePlaying;
         break;
      case ending:
         mpStateClass = mpStateEnding;
         break;
      case paused:
         mpStateClass = mpStatePaused;
         break;
      case endingpaused:
         mpStateClass = mpStateEndingPaused;
         break;
   }

   if( !mpStateClass->enter() )
   {
      mpStateClass = 0;
      return false;
   }

   return true;
}


enum PlayerFSM::eState PlayerFSM::getState()
{
   return mState;
}


void PlayerFSM::handleDerMixD( const QString &msg )
{
   tState lastState = mState;
   if( mpStateClass )
   {
      tState state = mpStateClass->handleDerMixD( msg );

      if( state != lastState )
      {
         changeState( state );
      }
   }
}
