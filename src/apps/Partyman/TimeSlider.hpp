/**
 * src/apps/Partyman/PlayerWidget.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef TIMESLIDER_HPP
#define TIMESLIDER_HPP TIMESLIDER_HPP

/* base class */
#include <QSlider>

/* system headers */

/* Qt headers */
#include <QTimer>

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */

/* forward declaration of local classes */


/* slightly modify the behaviour of the slider */
class TimeSlider : public QSlider
{
public:
   TimeSlider( Qt::Orientation orientation, QWidget *parent = 0 );

protected:
   /*  */
   void mousePressEvent( QMouseEvent *event );
   /*  */
   void mouseReleaseEvent( QMouseEvent *event );
   /*  */
   void wheelEvent( QWheelEvent *event );
   /*  */
   void keyPressEvent( QKeyEvent *event );
   /*  */
   void keyReleaseEvent( QKeyEvent *event );

private:
   TimeSlider( const TimeSlider &other );
   TimeSlider &operator=( const TimeSlider &other );

   QTimer mWheelTimeout;
};

#endif
