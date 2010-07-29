/**
 * src/apps/Partyman/PlayerFSMEnding.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef PLAYERFSMENDING_HPP
#define PLAYERFSMENDING_HPP PLAYERFSMENDING_HPP

/* base class */
#include "PlayerFSMBase.hpp"

/* system headers */

/* Qt headers */

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */

/* forward declaration of local classes */


class PlayerFSMEnding : public PlayerFSMBase
{
public:
   PlayerFSMEnding( PlayerWidget *playerWidget );
   
   /* called when state is entered, returns success */
   bool enter();
   /* called when state is left */
   void leave();
   /* called when DerMixD sends some output, returns state change request */
   PlayerFSM::tState handleDerMixD( const QString &msg );
   
private:
   PlayerFSMEnding( const PlayerFSMEnding &other );
   PlayerFSMEnding &operator=( const PlayerFSMEnding &other );
};

#endif

