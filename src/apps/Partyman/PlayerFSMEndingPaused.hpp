/**
 * src/apps/Partyman/PlayerFSMEndingPaused.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef PLAYERFSMENDINGPAUSED_HPP
#define PLAYERFSMENDINGPAUSED_HPP PLAYERFSMENDINGPAUSED_HPP

/* base class */
#include "PlayerFSMBase.hpp"

/* system headers */

/* Qt headers */

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */

/* forward declaration of local classes */


class PlayerFSMEndingPaused : public PlayerFSMBase
{
public:
   PlayerFSMEndingPaused( PlayerWidget *playerWidget );

   /* called when state is entered, returns success */
   bool enter();
   /* called when state is left */
   void leave();
   /* called when DerMixD sends some output, returns state change request */
   PlayerFSM::tState handleDerMixD( const QString &msg );

private:
   PlayerFSMEndingPaused( const PlayerFSMEndingPaused &that );
   PlayerFSMEndingPaused &operator=( const PlayerFSMEndingPaused &that );
};

#endif

