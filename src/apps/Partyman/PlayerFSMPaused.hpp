/**
 * src/apps/Partyman/PlayerFSMPaused.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef PLAYERFSMPAUSED_HPP
#define PLAYERFSMPAUSED_HPP PLAYERFSMPAUSED_HPP

/* base class */
#include "PlayerFSMBase.hpp"

/* system headers */

/* Qt headers */

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */

/* forward declaration of local classes */


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
   PlayerFSMPaused( const PlayerFSMPaused &that );
   PlayerFSMPaused &operator=( const PlayerFSMPaused &that );
};

#endif

