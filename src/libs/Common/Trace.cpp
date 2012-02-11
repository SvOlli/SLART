/*
 * src/libs/Common/Trace.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

/* class declaration */
#include "Trace.hpp"

/* system headers */
#ifndef Q_OS_WIN32
#include <cstdlib>
#include <execinfo.h>
#endif

/* Qt headers */

/* local library headers */

/* local headers */

/* class variables */
volatile unsigned int Trace::mDepth = 0;


Trace::Trace( const QString &className )
: mClassName( className )
{
mDepth++;
qDebug() << QString(mDepth,'>').toLocal8Bit().constData() << mClassName.toLocal8Bit().constData();
}

Trace::~Trace()
{
qDebug() << QString(mDepth,'<').toLocal8Bit().constData() << mClassName.toLocal8Bit().constData();
mDepth--;
}

const char* Trace::indent(QChar c)
{
   return QString(mDepth,c).toLocal8Bit().data();
}


#ifndef Q_OS_WIN32
QString Trace::backtrace()
{
   QString bt("BACKTRACE:");
   void **pointer = (void**)malloc(sizeof(void*)*256);
   int  size      = ::backtrace( pointer, 256 );
   char **symbols = ::backtrace_symbols( pointer, size );
   for( int i = 1; i < size; i++ )
   {
      bt.append( "\n" );
      bt.append( symbols[i] );
   }
   free( pointer );
   free( symbols );
   return bt;
}
#endif
