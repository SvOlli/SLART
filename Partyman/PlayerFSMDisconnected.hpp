/**
 * PlayerFSMDisconnected.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 */

#ifndef PLAYERFSMDISCONNECTED_HPP
#define PLAYERFSMDISCONNECTED_HPP PLAYERFSMDISCONNECTED_HPP

#include "PlayerFSMBase.hpp"

class PlayerFSMDisconnected : public PlayerFSMBase
{
public:
   PlayerFSMDisconnected( PlayerWidget *playerWidget );

   bool enter();
   void leave();
   PlayerFSM::tState handleDerMixD( const QString &msg );

private:
   PlayerFSMDisconnected( const PlayerFSMDisconnected &other );
   PlayerFSMDisconnected &operator=( const PlayerFSMDisconnected &other );
};

#endif

