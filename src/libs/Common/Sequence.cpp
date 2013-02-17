/*
 * src/libs/Common/Sequence.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

/* class declaration */
#include "Sequence.hpp"

/* system headers */

/* Qt headers */
#include <QMutexLocker>

/* local library headers */
#include <SingleInstance.hpp>

/* local headers */


Sequence::Sequence( QObject *parent )
: QObject( parent )
, mSequence( 1 )
, mMutex()
{
}


Sequence::Sequence( unsigned int start, QObject *parent )
: QObject( parent )
, mSequence( start )
, mMutex()
{
}


Sequence::~Sequence()
{
}


unsigned int Sequence::get()
{
   QMutexLocker l( &mMutex );
   emit current( mSequence );
   return mSequence++;
}


unsigned int Sequence::global()
{
   return SingleInstance::get<Sequence>( "Sequence" )->get();
}
