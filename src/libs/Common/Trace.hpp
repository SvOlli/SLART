/**
 * src/libs/Common/Trace.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef TRACE_HPP
#define TRACE_HPP TRACE_HPP

/* system headers */

/* Qt headers */
#include <QString>
#include <QtDebug>

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */

/* forward declaration of local classes */


/**************************************************/
/* these macros provide the tracing functionality */
/**************************************************/
/* generate the trace object, insert 'TRACEOBJECT(class::function)' as first line of function */
#define TRACESTART(x) Trace TRACEOBJECT(#x);
/* write a message with 'TRACEMSG << "message";' */
#define TRACEMSG qDebug() << Trace::indent(' ') << TRACEOBJECT.mClassName.toLocal8Bit().data()
/* write the current position with 'TRACECP' */
#define TRACECP  qDebug() << Trace::indent('#') << __FILE__ << ":" << __LINE__;
/* don't use the tracing class directly */
#ifndef Q_OS_WIN32
#include <csignal>
#define TRACEALERT ::raise(SIGUSR1);
#else
#define TRACEALERT qDebug() << "!!!" << __FILE__ << ":" << __LINE__;
#endif


/* backend */
class Trace
{
public:
   Trace( const QString &className );
   virtual ~Trace();
   static const char* indent(QChar c);
   const QString mClassName;
private:
   static volatile unsigned int mDepth;
};

#endif
