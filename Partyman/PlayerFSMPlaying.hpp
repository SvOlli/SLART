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
   
   /* called when state is entered, returns success */
   bool enter();
   /* called when state is left */
   void leave();
   /* called when DerMixD sends some output, returns state change request */
   PlayerFSM::tState handleDerMixD( const QString &msg );
   
private:
   PlayerFSMPlaying( const PlayerFSMPlaying &other );
   PlayerFSMPlaying &operator=( const PlayerFSMPlaying &other );
};

#endif

