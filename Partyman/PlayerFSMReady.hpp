/**
 * PlayerFSMReady.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 */

#ifndef PLAYERFSMREADY_HPP
#define PLAYERFSMREADY_HPP PLAYERFSMREADY_HPP

#include "PlayerFSMBase.hpp"

class PlayerFSMReady : public PlayerFSMBase
{
public:
   PlayerFSMReady( PlayerWidget *playerWidget );

   bool enter();
   void leave();
   PlayerFSM::tState handleDerMixD( const QString &msg );

private:
   PlayerFSMReady( const PlayerFSMReady &other );
   PlayerFSMReady &operator=( const PlayerFSMReady &other );
};

#endif

