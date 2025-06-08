/*
 * src/apps/Partyman/PlayerFSM.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "PlayerFSM.hpp"

/* system headers */

/* Qt headers */
#include <QProcess>

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
, mpNotifier( 0 )
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


void PlayerFSM::setNotifier( QProcess *notifier )
{
   mpNotifier = notifier;
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

   if( mpNotifier )
   {
      QProcessEnvironment env( mpNotifier->processEnvironment() );
      const char *stateString = getStateString();
      if( stateString )
      {
         env.insert( "STATE", stateString );
         mpNotifier->setProcessEnvironment( env );
         if( mpNotifier->state() != QProcess::NotRunning )
         {
            mpNotifier->terminate();
         }
         mpNotifier->start();
         mpNotifier->closeReadChannel( QProcess::StandardError );
         mpNotifier->closeReadChannel( QProcess::StandardOutput );
         mpNotifier->closeWriteChannel();
      }
   }

   return true;
}


enum PlayerFSM::eState PlayerFSM::getState()
{
   return mState;
}


const char *PlayerFSM::getStateString()
{
   const char *states[] =
   {
      "disconnected",
      "searching",
      "loading",
      "ready",
      "playing",
      "ending",
      "paused",
      "endingpaused"
   };
   switch( mState )
   {
      case disconnected:
         return states[0];
         break;
      case searching:
         return states[1];
         break;
      case loading:
         return states[2];
         break;
      case ready:
         return states[3];
         break;
      case playing:
         return states[4];
         break;
      case ending:
         return states[5];
         break;
      case paused:
         return states[6];
         break;
      case endingpaused:
         return states[7];
         break;
   }
   return (const char*)0;
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
