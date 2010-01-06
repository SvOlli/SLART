/**
 * EventHandler.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 */

#include "EventHandler.hpp"
#include "MySettings.hpp"

#include <QtGui>
#include <QShortcut>
#include <globalshortcutmanager.h>

#include <unistd.h>
#include "Trace.hpp"

EventHandler::EventHandler( QWidget *parent , Qt::WindowFlags flags )
: QMainWindow( parent, flags )
{
   QLabel *pLogo = new QLabel( QString("Global Key Catcher"), this );
   pLogo->setAlignment( Qt::AlignCenter );
   pLogo->setFrameShadow( QFrame::Raised );
   pLogo->setFrameShape( QFrame::Box );
   setCentralWidget( pLogo );
   
   GlobalShortcutManager *gsm = GlobalShortcutManager::instance();
//   gsm->connect( QKeySequence(Qt::Key_R),       this, SLOT(handleShortcut()) );
//   gsm->connect( QKeySequence(Qt::Key_F),       this, SLOT(handleShortcut()) );
   gsm->connect( QKeySequence(Qt::Key_F13),     this, SLOT(actionPause()) );
   gsm->connect( QKeySequence(Qt::Key_F14),     this, SLOT(actionNext()) );
   gsm->connect( QKeySequence(Qt::Key_Help),    this, SLOT(actionNext()) );
   gsm->connect( QKeySequence(Qt::Key_Execute), this, SLOT(actionPower()) );
}


EventHandler::~EventHandler()
{
   GlobalShortcutManager *gsm = GlobalShortcutManager::instance();
//   gsm->disconnect( QKeySequence(Qt::Key_R),       this, SLOT(handleShortcut()) );
//   gsm->disconnect( QKeySequence(Qt::Key_F),       this, SLOT(handleShortcut()) );
   gsm->disconnect( QKeySequence(Qt::Key_F13),     this, SLOT(actionPause()) );
   gsm->disconnect( QKeySequence(Qt::Key_F14),     this, SLOT(actionNext()) );
   gsm->disconnect( QKeySequence(Qt::Key_Help),    this, SLOT(actionNext()) );
   gsm->disconnect( QKeySequence(Qt::Key_Execute), this, SLOT(actionPower()) );
}


void EventHandler::actionNext()
{
   const QString message("P0N");
   const QString application("Partyman");
   MySettings().sendUdpMessage( message, application );
}


void EventHandler::actionPause()
{
   const QString message("P0A");
   const QString application("Partyman");
   MySettings().sendUdpMessage( message, application );
}


void EventHandler::actionPower()
{
   execl( "/usr/local/sbin/partyman-shutdown.sh", "/usr/local/sbin/partyman-shutdown.sh", (char *)0 );
}
