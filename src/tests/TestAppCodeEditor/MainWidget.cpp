/*
 * src/tests/TestAppCodeEditor/MainWidget.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

/* class declaration */
#include "MainWidget.hpp"

/* system headers */

/* Qt headers */
#include <QtGui>

/* local library headers */
#include <CodeEditor.hpp>
#include <LuaEditorDialog.hpp>
#include <MyLua.hpp>
#include <SingleInstance.hpp>
#include <SyntaxHighlighterLua.hpp>
#include <TagMap.hpp>

/* local headers */


MainWidget::MainWidget( QWidget *parent , Qt::WindowFlags flags )
: QWidget( parent, flags )
, mpLua( SingleInstance::get<MyLua>( "MyLua", false ) )
, mpOpenEditorButton( LuaEditorDialog::newEditorButton( "Test", this ) )
//, mpMessageBuffer( new QListWidget( this ) )
, mMessageBufferAlternateColor( false )
{
   QGridLayout *mainLayout   = new QGridLayout( this );
   mainLayout->setContentsMargins( 3, 3, 3, 3 );
   mainLayout->addWidget( mpOpenEditorButton, 0, 0 );

   mpLua->start();
}


MainWidget::~MainWidget()
{
   if( mpLua->isRunning() )
   {
      QMetaObject::invokeMethod( mpLua, "quit" );
      mpLua->wait( 1000 );
   }
   mpLua->deleteLater();
}


void MainWidget::log( const QString &msg, bool error )
{
   QListWidgetItem *item = new QListWidgetItem( msg, mpMessageBuffer );
   if( error )
   {
      item->setForeground( Qt::red );
   }
   if( mMessageBufferAlternateColor )
   {
      item->setBackground( QBrush( mpMessageBuffer->palette().color( QPalette::AlternateBase ) ) );
   }
   mMessageBufferAlternateColor = !mMessageBufferAlternateColor;
   mpMessageBuffer->addItem( item );

   while( mpMessageBuffer->count() > 500 )
   {
      item = mpMessageBuffer->takeItem(0);
      if( item )
      {
         delete item;
      }
   }

   mpMessageBuffer->scrollToBottom();
}
