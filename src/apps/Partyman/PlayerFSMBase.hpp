/**
 * src/apps/Partyman/PlayerFSMBase.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef PLAYERFSMBASE_HPP
#define PLAYERFSMBASE_HPP PLAYERFSMBASE_HPP

#include <QLabel>

#include "PlayerWidget.hpp"
#include "PlayerFSM.hpp"


class PlayerFSMBase
{
public:
   PlayerFSMBase( PlayerWidget *playerWidget );
   virtual ~PlayerFSMBase();
   
   /* called when state is entered, returns success */
   virtual bool enter() = 0;
   /* called when state is left */
   virtual void leave() = 0;
   /* called when DerMixD sends some output, returns state change request */
   virtual PlayerFSM::tState handleDerMixD( const QString &msg ) = 0;
   
private:
   PlayerFSMBase( const PlayerFSMBase &other );
   PlayerFSMBase &operator=( const PlayerFSMBase &other );
   
protected:
   PlayerWidget *mpPlayerWidget;
};

#endif

