/**
 * PlayerFSMPlaying.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 */

#ifndef PLAYERFSMPLAYING_HPP
#define PLAYERFSMPLAYING_HPP PLAYERFSMPLAYING_HPP

#include "PlayerFSMBase.hpp"

class PlayerFSMPlaying : public PlayerFSMBase
{
public:
   PlayerFSMPlaying( PlayerWidget *playerWidget );

   bool enter();
   void leave();
   PlayerFSM::tState handleDerMixD( const QString &msg );

private:
   PlayerFSMPlaying( const PlayerFSMPlaying &other );
   PlayerFSMPlaying &operator=( const PlayerFSMPlaying &other );
};

#endif

