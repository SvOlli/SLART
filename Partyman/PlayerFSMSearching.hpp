/**
 * PlayerFSMSearching.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 */

#ifndef PLAYERFSMSEARCHING_HPP
#define PLAYERFSMSEARCHING_HPP PLAYERFSMSEARCHING_HPP

#include "PlayerFSMBase.hpp"

class PlayerFSMSearching : public PlayerFSMBase
{
public:
   PlayerFSMSearching( PlayerWidget *playerWidget );

   bool enter();
   void leave();
   PlayerFSM::tState handleDerMixD( const QString &msg );

private:
   PlayerFSMSearching( const PlayerFSMSearching &other );
   PlayerFSMSearching &operator=( const PlayerFSMSearching &other );
};

#endif

