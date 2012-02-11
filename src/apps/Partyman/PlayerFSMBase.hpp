/*
 * src/apps/Partyman/PlayerFSMBase.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef PLAYERFSMBASE_HPP
#define PLAYERFSMBASE_HPP PLAYERFSMBASE_HPP

/* base class */

/* system headers */

/* Qt headers */
#include <QLabel>

/* local library headers */

/* local headers */
#include "PlayerWidget.hpp"
#include "PlayerFSM.hpp"

/* forward declaration of Qt classes */

/* forward declaration of local classes */


/*!
  \addtogroup Partyman

  @{
*/

/*!
 \brief TODO

*/
class PlayerFSMBase
{
public:
   PlayerFSMBase( PlayerWidget *playerWidget );
   virtual ~PlayerFSMBase();

   /*!
    \brief called when state is entered, returns success

   */
   virtual bool enter() = 0;
   /*!
    \brief called when state is left

   */
   virtual void leave() = 0;
   /*!
    \brief called when DerMixD sends some output, returns state change request

   */
   virtual PlayerFSM::tState handleDerMixD( const QString &msg ) = 0;

private:
   PlayerFSMBase( const PlayerFSMBase &that );
   PlayerFSMBase &operator=( const PlayerFSMBase &that );

protected:
   PlayerWidget *mpPlayerWidget;
};

/*! @} */

#endif

