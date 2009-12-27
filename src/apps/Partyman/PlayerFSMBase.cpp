/**
 * PlayerFSMBase.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
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
