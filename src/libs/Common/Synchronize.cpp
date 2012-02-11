/*
 * src/libs/Common/Synchronize.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

/* class declaration */
#include "Synchronize.hpp"

/* system headers */

/* Qt headers */
#include <QtEndian>
#include <QByteArray>

/* local library headers */

/* local headers */

/* class variables */


Synchronize::Synchronize()
: mMutex()
, mWaitCondition()
{
}


Synchronize::~Synchronize()
{
}


bool Synchronize::wait()
{
   return mWaitCondition.wait( &mMutex );
}


void Synchronize::release()
{
   mWaitCondition.wakeAll();
}
