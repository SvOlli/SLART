/*
 * src/apps/Partyman/PlayerFSMDisconnected.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef PLAYERFSMDISCONNECTED_HPP
#define PLAYERFSMDISCONNECTED_HPP PLAYERFSMDISCONNECTED_HPP

/* base class */
#include "PlayerFSMBase.hpp"

/* system headers */

/* Qt headers */

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */

/* forward declaration of local classes */


/*!
  \addtogroup Partyman

  @{
*/

/*!
 \brief \todo complete documentation

*/
class PlayerFSMDisconnected : public PlayerFSMBase
{
public:
   PlayerFSMDisconnected( PlayerWidget *playerWidget );

   /*!
    \brief called when state is entered, returns success

   */
   bool enter();
   /*!
    \brief called when state is left

   */
   void leave();
   /*!
    \brief called when DerMixD sends some output, returns state change request

   */
   PlayerFSM::tState handleDerMixD( const QString &msg );

private:
   PlayerFSMDisconnected( const PlayerFSMDisconnected &that );
   PlayerFSMDisconnected &operator=( const PlayerFSMDisconnected &that );
};

/*! @} */

#endif

