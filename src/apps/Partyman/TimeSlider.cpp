/*
 * src/apps/Partyman/PlayerWidget.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "TimeSlider.hpp"

/* system headers */

/* Qt headers */
#include <QTimer>

/* local library headers */
#include <MyMouseEvent.hpp>

/* local headers */


TimeSlider::TimeSlider( Qt::Orientation orientation, QWidget *parent )
: QSlider( orientation, parent )
, mpWheelTimeout( new QTimer( this ) )
{
   mpWheelTimeout->setSingleShot( true );
   mpWheelTimeout->setInterval( 333 );
   setInvertedControls( true );
   connect( mpWheelTimeout, SIGNAL(timeout()),
            this, SIGNAL(sliderReleased()) );
}


void TimeSlider::mousePressEvent( QMouseEvent *event )
{
   if( event->button() == Qt::LeftButton )
   {
      MyMouseEvent *myevent = reinterpret_cast<MyMouseEvent*>(event);
      myevent->setMouseButton( Qt::MidButton );
   }
   QSlider::mousePressEvent( event );
}


void TimeSlider::mouseReleaseEvent( QMouseEvent *event )
{
   if( event->button() == Qt::LeftButton )
   {
      MyMouseEvent *myevent = reinterpret_cast<MyMouseEvent*>(event);
      myevent->setMouseButton( Qt::MidButton );
   }
   QSlider::mouseReleaseEvent( event );
}


void TimeSlider::wheelEvent( QWheelEvent *event )
{
   if( !mpWheelTimeout->isActive() )
   {
      emit sliderPressed();
   }
   mpWheelTimeout->start();
   QSlider::wheelEvent( event );
}


void TimeSlider::keyPressEvent( QKeyEvent *event )
{
   QSlider::keyPressEvent( event );
   if( !event->isAutoRepeat() )
   {
      emit sliderPressed();
   }
}


void TimeSlider::keyReleaseEvent( QKeyEvent *event )
{
   QSlider::keyReleaseEvent( event );
   if( !event->isAutoRepeat() )
   {
      emit sliderReleased();
   }
}
