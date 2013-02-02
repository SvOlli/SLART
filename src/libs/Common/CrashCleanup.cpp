/*
 * src/libs/Common/CrashCleanup.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

/* class declaration */
#include "CrashCleanup.hpp"

/* system headers */
#include <csignal>
#include <cstdlib>

/* Qt headers */
#include <QObject>

/* local library headers */

/* local headers */


QList<QObject*> CrashCleanup::cObjects;


static void setSignals( __sighandler_t signalHandler )
{
#if (!defined _WIN32) || (defined __MINGW32__)
   int i;
   for( i = 1; i < 32; i++ )
   {
      switch( i )
      {
         // numeric because of MinGW lacking most signals
      case  9: //SIGKILL
      case 13: //SIGPIPE
      case 17: //SIGCHLD
      case 19: //SIGSTOP
         break;
      default:
         ::signal( i, signalHandler );
         break;
      }
   }
#else
#warning signal handler not implemented for this platform
#endif
}


void CrashCleanup::addObject( QObject *object )
{
   if( !cObjects.size() )
   {
      /* first object, so install signal handler */
      setSignals( (__sighandler_t)(CrashCleanup::runCleanup) );
   }
   if( !cObjects.contains( object ) )
   {
      cObjects.prepend( object );
   }
}


void CrashCleanup::removeObject( QObject *object )
{
   cObjects.removeOne( object );
   if( !cObjects.count() )
   {
      /* no one left, party's over, clean up */
      setSignals( SIG_DFL );
   }
}


void CrashCleanup::runCleanup( int signal )
{
   foreach( QObject *object, cObjects )
   {
      delete object;
   }
   /* re-raise the signal without handler */
   setSignals( SIG_DFL );
   ::raise( signal );
   /* should not get here */
   ::exit( signal );
}
