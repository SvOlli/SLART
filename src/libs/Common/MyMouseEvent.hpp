/**
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
    \brief a very nasty hack to change the protected button variables

   */
/*!
  \addtogroup Common

  @{
*/

/*!
 \brief TODO

*/
class MyMouseEvent : public QMouseEvent
{
   /*!
    \brief everything private, usage only via reinterpret_cast

   */
private:
   MyMouseEvent();
   MyMouseEvent( const MyMouseEvent &that );
   MyMouseEvent &operator=( const MyMouseEvent &that );
   virtual ~MyMouseEvent();
public:
   /*!
    \brief only cause for this class: modify the mouse button

   */
   void setMouseButton( Qt::MouseButton bt );
};

/*! @} */

#endif
