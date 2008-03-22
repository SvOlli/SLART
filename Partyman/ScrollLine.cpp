/**
 * ScrollLine.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#include "ScrollLine.hpp"
#include "GlobalConfigWidget.hpp"

#include <QtGui>

#include "Trace.hpp"

ScrollLine::ScrollLine( QWidget *parent )
: QLineEdit( parent )
, mpTimer( new QTimer( this ) )
, mDirection( 1 )
, mPosition( 0 )
{
   mpTimer->setSingleShot( false );
   mpTimer->setInterval( 333 );
   
   setReadOnly( true );
   setContextMenuPolicy( Qt::NoContextMenu );
   
   connect( mpTimer, SIGNAL(timeout()), this, SLOT(scrolling()) );
   mpTimer->start();
}


void ScrollLine::scrolling()
{
   if( mPosition < 0 )
   {
      mDirection = 1;
   }
   if( mPosition >= text().size() )
   {
      mDirection = -1;
   }
   mPosition += mDirection;
   setCursorPosition( mPosition );
}


void ScrollLine::setText( const QString &text )
{
   QLineEdit::setText( text );
   mDirection = 1;
   mPosition = 0;
   setCursorPosition( mPosition );
   clearFocus();
}


void ScrollLine::focusInEvent( QFocusEvent *event )
{
   if( event->gotFocus() )
   {
      GlobalConfigWidget::setClipboard( QLineEdit::toolTip() );
   }
   event->ignore();
}
