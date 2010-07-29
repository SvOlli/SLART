/**
 * src/apps/Partyman/PlayerFSM.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef PLAYERFSM_HPP
#define PLAYERFSM_HPP PLAYERFSM_HPP

/* base class */

/* system headers */

/* Qt headers */
#include <QMutex>
#include <QTcpSocket>
#include <QWidget>

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */

/* forward declaration of local classes */
class PlayerFSMBase;
class PlayerFSMDisconnected;
class PlayerFSMSearching;
class PlayerFSMLoading;
class PlayerFSMReady;
class PlayerFSMEnding;
class PlayerFSMPlaying;
class PlayerFSMPaused;
class PlayerFSMEndingPaused;
class PlayerWidget;


class PlayerFSM
{
public:
   enum eState { disconnected, searching, loading, ready, 
                 playing, ending, paused, endingpaused };
   typedef enum eState tState;

   PlayerFSM( PlayerWidget *playerWidget );
   virtual ~PlayerFSM();
   
   /* change the state of the FSM */
   bool changeState( tState newState );
   /* get the state of the FSM */
   tState getState();
   /* handle messages from DerMixD */
   void handleDerMixD( const QString &msg );

private:
   PlayerFSM( const PlayerFSM &other );
   PlayerFSM &operator=( const PlayerFSM &other );

   enum eState                  mState;
   PlayerFSMDisconnected        *mpStateDisconnected;
   PlayerFSMSearching           *mpStateSearching;
   PlayerFSMLoading             *mpStateLoading;
   PlayerFSMReady               *mpStateReady;
   PlayerFSMPlaying             *mpStatePlaying;
   PlayerFSMEnding              *mpStateEnding;
   PlayerFSMPaused              *mpStatePaused;
   PlayerFSMEndingPaused        *mpStateEndingPaused;
   PlayerFSMBase                *mpStateClass;
};

#endif
