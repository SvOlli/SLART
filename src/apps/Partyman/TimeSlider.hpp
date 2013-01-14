/*
 * src/apps/Partyman/PlayerWidget.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
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


/*!
  \addtogroup Partyman

  @{
*/

/*!
 \brief slightly modify the behaviour of the slider

 \dotfile "graphs/apps/Partyman/TimerSlider_connect.dot" "Connect Graph"
*/
class TimeSlider : public QSlider
{
public:
   TimeSlider( Qt::Orientation orientation, QWidget *parent = 0 );

protected:
   /*!
    \brief reimplemented for mapping of mouse button 2 to 3

   */
   void mousePressEvent( QMouseEvent *event );
   /*!
    \brief reimplemented for mapping of mouse button 2 to 3

   */
   void mouseReleaseEvent( QMouseEvent *event );
   /*!
    \brief reimplemented for timeout handling

   */
   void wheelEvent( QWheelEvent *event );
   /*!
    \brief reimplemented for handling of auto-repeat

   */
   void keyPressEvent( QKeyEvent *event );
   /*!
    \brief reimplemented for handling of auto-repeat

   */
   void keyReleaseEvent( QKeyEvent *event );

private:
   Q_DISABLE_COPY( TimeSlider )

   QTimer mWheelTimeout; /*!< \brief \todo complete documentation */
};

/*! @} */

#endif
