/*
 * src/libs/Common/ScrollLine.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

/* class declaration */
#include "ScrollLine.hpp"

/* system headers */

/* Qt headers */
#include <QtGui>

/* local library headers */

/* local headers */
#include "GlobalConfigWidget.hpp"
#include "MyMouseEvent.hpp"
#include "TrackInfo.hpp"

#include "Trace.hpp"


QTimer *ScrollLine::cpTimer = 0;

ScrollLine::ScrollLine( QWidget *parent, bool autoScroll )
: QLineEdit( parent )
, mClicked( false )
, mDirection( 1 )
, mPosition( 0 )
, mpDragInfo( new DnDFileInfo( this ) )
{
   setReadOnly( true );
   setContextMenuPolicy( Qt::NoContextMenu );

   if( autoScroll )
   {
      if( !cpTimer )
      {
         cpTimer = new QTimer( qApp );
         cpTimer->setSingleShot( false );
         cpTimer->setInterval( 333 );
         cpTimer->start();
      }
      connect( cpTimer, SIGNAL(timeout()),
               this, SLOT(scrolling()) );
   }
}


ScrollLine::~ScrollLine()
{
}


void ScrollLine::setDragFileName( const QString &fileName )
{
   mpDragInfo->setFilePath( fileName );
}


void ScrollLine::setDragInfo( const TrackInfo &trackInfo )
{
   mpDragInfo->setTrackInfo( trackInfo );
}


void ScrollLine::scrolling()
{
   if( mClicked )
   {
      return;
   }

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


void ScrollLine::paintEvent( QPaintEvent *event )
{
   QPalette myPalette( palette() );
   myPalette.setColor( QPalette::Base, myPalette.color( QPalette::Window ) );
   setPalette( myPalette );
   QLineEdit::paintEvent( event );
}


void ScrollLine::mouseDoubleClickEvent( QMouseEvent *event )
{
   mClicked = true;
   setCursorPosition( cursorPositionAt( event->pos() ) );
   QLineEdit::mouseDoubleClickEvent( event );
}


void ScrollLine::mousePressEvent( QMouseEvent *event )
{
   mClicked = true;
   QString dragFileName( mpDragInfo->filePath() );
   if( dragFileName.isEmpty() )
   {
      setCursorPosition( cursorPositionAt( event->pos() ) );
      QLineEdit::mousePressEvent( event );
   }
   else
   {
      QDrag *drag = mpDragInfo->createDrag();
      drag->exec( Qt::CopyAction, Qt::CopyAction );
      mClicked = false;
   }
}


void ScrollLine::mouseReleaseEvent( QMouseEvent *event )
{
   mClicked = false;
   GlobalConfigWidget::setClipboard( selectedText() );
   setCursorPosition( cursorPositionAt( event->pos() ) );
   QLineEdit::mouseReleaseEvent( event );
}
