/**
 * PlayerFSMPaused.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 */

#ifndef PLAYERFSMPAUSED_HPP
#define PLAYERFSMPAUSED_HPP PLAYERFSMPAUSED_HPP

#include "PlayerFSMBase.hpp"

class PlayerFSMPaused : public PlayerFSMBase
{
public:
   PlayerFSMPaused( PlayerWidget *playerWidget );

   bool enter();
   void leave();
   PlayerFSM::tState handleDerMixD( const QString &msg );

private:
   PlayerFSMPaused( const PlayerFSMPaused &other );
   PlayerFSMPaused &operator=( const PlayerFSMPaused &other );
};

#endif

