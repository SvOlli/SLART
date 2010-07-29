/**
 * src/apps/Partyman/PlayerWidget.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
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


/* a very nasty hack to change the protected button variables */
class MyMouseEvent : public QMouseEvent
{
private:
   MyMouseEvent();
   MyMouseEvent &operator=( const MyMouseEvent &other );
   virtual ~MyMouseEvent();
public:
   void setMouseButton( Qt::MouseButton bt );
};

#endif
