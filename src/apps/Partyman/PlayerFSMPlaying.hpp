/*
 * src/apps/Partyman/PlayerFSMPlaying.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef PLAYERFSMPLAYING_HPP
#define PLAYERFSMPLAYING_HPP PLAYERFSMPLAYING_HPP

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
class PlayerFSMPlaying : public PlayerFSMBase
{
public:
   PlayerFSMPlaying( PlayerWidget *playerWidget );

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
   PlayerFSMPlaying( const PlayerFSMPlaying &that );
   PlayerFSMPlaying &operator=( const PlayerFSMPlaying &that );
};

/*! @} */

#endif

