/**
 * src/libs/Common/Synchronize.hpp
 * written by Sven Oliver Moll
*
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef SYNCHONIZE_HPP
#define SYNCHONIZE_HPP SYNCHONIZE_HPP

/* system headers */

/* Qt headers */
#include <QMutex>
#include <QWaitCondition>

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */

/* forward declaration of local classes */


class Synchronize
{
public:
   Synchronize();
   virtual ~Synchronize();

   /* wait until someone else calls release() */
   bool wait();
   /* let the waiting call continue */
   void release();

private:
   Synchronize( const Synchronize &other );
   Synchronize &operator=( const Synchronize &other );

   QMutex         mMutex;
   QWaitCondition mWaitCondition;
};

#endif
