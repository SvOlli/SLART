/**
 * PlayerFSMLoading.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 */

#ifndef PLAYERFSMLOADING_HPP
#define PLAYERFSMLOADING_HPP PLAYERFSMLOADING_HPP

#include "PlayerFSMBase.hpp"

class PlayerFSMLoading : public PlayerFSMBase
{
public:
   PlayerFSMLoading( PlayerWidget *playerWidget );

   /* called when state is entered, returns success */
   bool enter();
   /* called when state is left */
   void leave();
   /* called when DerMixD sends some output, returns state change request */
   PlayerFSM::tState handleDerMixD( const QString &msg );

private:
   bool mInScan;
   
   PlayerFSMLoading( const PlayerFSMLoading &other );
   PlayerFSMLoading &operator=( const PlayerFSMLoading &other );
};

#endif

