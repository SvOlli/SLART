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

   bool enter();
   void leave();
   PlayerFSM::tState handleDerMixD( const QString &msg );

private:
   PlayerFSMEndingPaused( const PlayerFSMEndingPaused &other );
   PlayerFSMEndingPaused &operator=( const PlayerFSMEndingPaused &other );
};

#endif

