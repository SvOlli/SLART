/*
 * src/apps/Partyman/MyMouseEvent.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
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
