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


CrashCleanup *CrashCleanup::cpHandler = 0;


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


void CrashCleanup::create()
{
   if( !cpHandler )
   {
      cpHandler = new CrashCleanup();
   }
   setSignals( (__sighandler_t)(CrashCleanup::runCleanup) );
}


void CrashCleanup::destroy()
{
   if( cpHandler )
   {
      delete cpHandler;
      cpHandler = 0;
   }
   setSignals( (__sighandler_t)(SIG_DFL) );
}


void CrashCleanup::addObject( QObject *object )
{
   cpHandler->mObjects.prepend( object );
}


void CrashCleanup::removeObject( QObject *object )
{
   cpHandler->mObjects.removeOne( object );
}


void CrashCleanup::runCleanup( int signal )
{
   foreach( QObject *object, cpHandler->mObjects )
   {
      delete object;
   }
   ::exit( signal );
}


CrashCleanup::CrashCleanup()
: mObjects()
{
}


CrashCleanup::~CrashCleanup()
{
}
