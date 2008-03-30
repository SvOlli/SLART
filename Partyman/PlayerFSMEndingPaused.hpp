/**
 * PlayerFSMEndingPaused.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 */

#ifndef PLAYERFSMENDINGPAUSED_HPP
#define PLAYERFSMENDINGPAUSED_HPP PLAYERFSMENDINGPAUSED_HPP

#include "PlayerFSMBase.hpp"

class PlayerFSMEndingPaused : public PlayerFSMBase
{
public:
   PlayerFSMEndingPaused( PlayerWidget *playerWidget );

   /* called when state is entered, returns success */
   bool enter();
   /* called when state is left */
   void leave();
   /* called when DerMixD sends some output, returns state change request */
   PlayerFSM::tState handleDerMixD( const QString &msg );

private:
   PlayerFSMEndingPaused( const PlayerFSMEndingPaused &other );
   PlayerFSMEndingPaused &operator=( const PlayerFSMEndingPaused &other );
};

#endif

