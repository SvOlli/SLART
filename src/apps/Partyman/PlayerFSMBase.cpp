/**
 * src/apps/Partyman/PlayerFSMBase.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#include "PlayerFSMBase.hpp"

#include "PlayerWidget.hpp"


PlayerFSMBase::PlayerFSMBase( PlayerWidget *playerWidget )
: mpPlayerWidget( playerWidget )
{
}


PlayerFSMBase::~PlayerFSMBase()
{
}
