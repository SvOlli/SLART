/**
 * MainWidget.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 */

#include "MainWidget.hpp"

#include <QtGui>
#include <QShortcut>
#include <globalshortcutmanager.h>


MainWidget::MainWidget( QWidget *parent , Qt::WindowFlags flags )
: QWidget( parent, flags )
, mpMessageBuffer( new QListWidget( this ) )
{
   QVBoxLayout *mainLayout   = new QVBoxLayout( this );
#if QT_VERSION < 0x040300
   mainLayout->setMargin( 3 );
#else
   mainLayout->setContentsMargins( 3, 3, 3, 3 );
#endif
   
   mainLayout->addWidget( mpMessageBuffer );

#if 0
   QShortcut *f5 = new QShortcut( QKeySequence(Qt::Key_F5), this );
   
   connect( f5, SIGNAL(activated()),
            this, SLOT(handleShortcut()) );
#else
   
   GlobalShortcutManager *gsm = GlobalShortcutManager::instance();
   gsm->connect( QKeySequence(Qt::Key_F5), this, SLOT(handleShortcut()) );
   
#endif
   
   setLayout( mainLayout );
}


void MainWidget::handleSLART( const QStringList &message )
{
   mpMessageBuffer->addItem( "> "+QDateTime::currentDateTime().toString() );

   for( int i = 0; i < message.size(); i++ )
   {
      mpMessageBuffer->addItem( message.at(i) );
   }

   while( mpMessageBuffer->count() > 100 )
   {
      QListWidgetItem *item = mpMessageBuffer->takeItem(0);
      if( item )
      {
         delete item;
      }
   }
   
   mpMessageBuffer->scrollToBottom();
}


void MainWidget::handleShortcut()
{
   static int count = 1;
   handleSLART( QStringList(QString::number( count++ )) );
}
