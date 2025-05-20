/*
 * src/libs/Common/Random.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

/* class declaration */
#include "Random.hpp"

/* system headers */
#include <time.h>

/* Qt headers */

/* local library headers */

/* local headers */

/* class variable instantiation */


Random *Random::cpRandom = 0;

QRandomGenerator *Random::get()
{
   if( !cpRandom )
   {
      cpRandom = new Random();
   }
   return &(cpRandom->mRandom);
}

Random::Random( QObject *parent )
: QObject( parent )
, mRandom( time((time_t*)0) )
{
   cpRandom = this;
}


Random::~Random()
{
   cpRandom = 0;
}


quint32 Random::generate()
{
   return Random::get()->generate();
}


quint32 Random::bounded( quint32 max )
{
   return Random::get()->bounded( max );
}
