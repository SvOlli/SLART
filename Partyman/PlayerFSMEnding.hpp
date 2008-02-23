/**
 * PlayerFSMEnding.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 */

#ifndef PLAYERFSMENDING_HPP
#define PLAYERFSMENDING_HPP PLAYERFSMENDING_HPP

#include "PlayerFSMBase.hpp"

class PlayerFSMEnding : public PlayerFSMBase
{
public:
   PlayerFSMEnding( PlayerWidget *playerWidget );

   bool enter();
   void leave();
   PlayerFSM::tState handleDerMixD( const QString &msg );

private:
   PlayerFSMEnding( const PlayerFSMEnding &other );
   PlayerFSMEnding &operator=( const PlayerFSMEnding &other );
};

#endif

