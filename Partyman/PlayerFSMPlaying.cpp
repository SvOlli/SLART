/**
 * PlayerFSMPlaying.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 */

#include "PlayerFSMPlaying.hpp"
#include "PlayerWidget.hpp"
#include "ControlWidget.hpp"
#include "ScrollLine.hpp"

#include <QSlider>


PlayerFSMPlaying::PlayerFSMPlaying( PlayerWidget *playerWidget )
: PlayerFSMBase( playerWidget )
{
}


bool PlayerFSMPlaying::enter()
{
   mpPlayerWidget->mpStatusDisplay->setText( QWidget::tr("playing") );
   mpPlayerWidget->sendCommand( "start" );
   mpPlayerWidget->sendCommand( "seek", QString::number(mpPlayerWidget->mpPlayPosition->value()) );
   mpPlayerWidget->mpControlWidget->log( "p0p", "play", mpPlayerWidget->mpScrollLine->toolTip() );
   mpPlayerWidget->sendCommand( "watch" );
   mpPlayerWidget->disablePlayPosition( false );

   return true;
}


void PlayerFSMPlaying::leave()
{
}


enum PlayerFSM::eState PlayerFSMPlaying::handleDerMixD( const QString &msg )
{
   if( msg.startsWith( QString("[ch")+QChar('0'+mpPlayerWidget->mPlayer)+']' ) )
   {
      mpPlayerWidget->updateTime( msg.mid(6) );
   }

   return PlayerFSM::playing;
}

