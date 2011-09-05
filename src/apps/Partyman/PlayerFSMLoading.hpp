/**
 * src/apps/Partyman/PlayerFSMLoading.hpp
 * written by Sven Oliver Moll
 *
 * available at http://www.gnu.org/licenses/gpl.html
 * distributed under the terms of the GNU Public License (GPL)
 */

#ifndef PLAYERFSMLOADING_HPP
#define PLAYERFSMLOADING_HPP PLAYERFSMLOADING_HPP

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
class PlayerFSMLoading : public PlayerFSMBase
{
public:
   PlayerFSMLoading( PlayerWidget *playerWidget );

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
   bool mInScan;

   PlayerFSMLoading( const PlayerFSMLoading &that );
   PlayerFSMLoading &operator=( const PlayerFSMLoading &that );
};

/*! @} */

#endif

