/*
 * src/apps/Partyman/PlayerFSMReady.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef PLAYERFSMREADY_HPP
#define PLAYERFSMREADY_HPP PLAYERFSMREADY_HPP

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
 \brief TODO

*/
class PlayerFSMReady : public PlayerFSMBase
{
public:
   PlayerFSMReady( PlayerWidget *playerWidget );

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
   PlayerFSMReady( const PlayerFSMReady &that );
   PlayerFSMReady &operator=( const PlayerFSMReady &that );
};

/*! @} */

#endif

