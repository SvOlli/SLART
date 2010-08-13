/**
 * src/apps/Partyman/MyMouseEvent.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "MyMouseEvent.hpp"

/* system headers */

/* Qt headers */

/* local library headers */

/* local headers */


/* a very nasty hack to change the protected button variables */
void MyMouseEvent::setMouseButton( Qt::MouseButton bt )
{
   if( mouseState & b )
   {
      mouseState &= ~b;
      mouseState |= bt;
   }
   b = bt;
}
