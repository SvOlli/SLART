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
   
   /* called when state is entered, returns success */
   bool enter();
   /* called when state is left */
   void leave();
   /* called when DerMixD sends some output, returns state change request */
   PlayerFSM::tState handleDerMixD( const QString &msg );
   
private:
   PlayerFSMReady( const PlayerFSMReady &other );
   PlayerFSMReady &operator=( const PlayerFSMReady &other );
};

#endif

