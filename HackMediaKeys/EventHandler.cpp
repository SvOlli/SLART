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
   gsm->connect( QKeySequence(Qt::Key_R),    this, SLOT(handleShortcut()) );
   gsm->connect( QKeySequence(Qt::Key_F),    this, SLOT(handleShortcut()) );
   gsm->connect( QKeySequence(Qt::Key_F13),  this, SLOT(handleShortcut()) );
   gsm->connect( QKeySequence(Qt::Key_F14),  this, SLOT(handleShortcut()) );
   gsm->connect( QKeySequence(Qt::Key_Help), this, SLOT(handleShortcut()) );
   
   hide();
}


EventHandler::~EventHandler()
{
   GlobalShortcutManager *gsm = GlobalShortcutManager::instance();
   gsm->disconnect( QKeySequence(Qt::Key_R),    this, SLOT(handleShortcut()) );
   gsm->disconnect( QKeySequence(Qt::Key_F),    this, SLOT(handleShortcut()) );
   gsm->disconnect( QKeySequence(Qt::Key_F13),  this, SLOT(handleShortcut()) );
   gsm->disconnect( QKeySequence(Qt::Key_F14),  this, SLOT(handleShortcut()) );
   gsm->disconnect( QKeySequence(Qt::Key_Help), this, SLOT(handleShortcut()) );
}


void EventHandler::handleShortcut()
{
   const QString message("P0N");
   const QString application("Partyman");
   MySettings().sendUdpMessage( message, application );
}
