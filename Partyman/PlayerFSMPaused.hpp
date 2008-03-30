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

   /* called when state is entered, returns success */
   bool enter();
   /* called when state is left */
   void leave();
   /* called when DerMixD sends some output, returns state change request */
   PlayerFSM::tState handleDerMixD( const QString &msg );

private:
   PlayerFSMPaused( const PlayerFSMPaused &other );
   PlayerFSMPaused &operator=( const PlayerFSMPaused &other );
};

#endif

