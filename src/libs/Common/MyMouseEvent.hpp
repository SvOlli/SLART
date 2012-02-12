/*
 * src/apps/Partyman/PlayerWidget.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef MYMOUSEEVENT_HPP
#define MYMOUSEEVENT_HPP MYMOUSEEVENT_HPP

/* base class */
#include <QMouseEvent>

/* system headers */

/* Qt headers */

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */

/* forward declaration of local classes */


/*!
  \addtogroup Common

  @{
*/

/*!
 \brief hack to change mouse button in QMouseEvent

 usage only via reinterpret_cast:
\code
class::mousePressEvent( QMouseEvent *event )
if( event->button() == Qt::LeftButton )
{
   MyMouseEvent *myevent = reinterpret_cast<MyMouseEvent*>(event);
   myevent->setMouseButton( Qt::MidButton );
}
\endcode
*/
class MyMouseEvent : public QMouseEvent
{
private:
   Q_DISABLE_COPY( MyMouseEvent )

public:
   /*!
    \brief only cause for this class: modify the mouse button

   */
   void setMouseButton( Qt::MouseButton bt );
};

/*! @} */

#endif
