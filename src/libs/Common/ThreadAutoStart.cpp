/*
 * src/libs/Common/ThreadAutoStart.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

/* class declaration */
#include "ThreadAutoStart.hpp"

/* system headers */

/* Qt headers */
#include <QCoreApplication>
#include <QThread>
#include <QTimer>

/* local library headers */

/* local headers */

/* class variables */


ThreadAutoStart::ThreadAutoStart( QThread *thread )
: QObject( QCoreApplication::instance() )
, mpThread( thread )
{
   QTimer::singleShot( 0, this, SLOT(start()) );
}


ThreadAutoStart::~ThreadAutoStart()
{
}


void ThreadAutoStart::start()
{
   mpThread->start();
   deleteLater();
}
