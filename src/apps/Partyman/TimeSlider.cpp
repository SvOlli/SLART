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

/* local library headers */
#include <MyMouseEvent.hpp>

/* local headers */


TimeSlider::TimeSlider( Qt::Orientation orientation, QWidget *parent )
: QSlider( orientation, parent )
{
   mWheelTimeout.setSingleShot( true );
   mWheelTimeout.setInterval( 333 );
   setInvertedControls( true );
   connect( &mWheelTimeout, SIGNAL(timeout()),
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
   if( !mWheelTimeout.isActive() )
   {
      emit sliderPressed();
   }
   mWheelTimeout.start();
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
