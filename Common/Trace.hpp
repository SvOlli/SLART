/**
 * Trace.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#ifndef TRACE_HPP
#define TRACE_HPP TRACE_HPP

#include <QString>
#include <qdebug.h>

#ifndef WIN32
#include <signal.h>
#define TRACEALERT ::raise(SIGUSR1);
#else
#define TRACEALERT qDebug() << "!!!" << __FILE__ << ":" << __LINE__;
#endif

#define TRACESTART(x) Trace TRACEOBJECT(#x);
#define TRACEMSG qDebug() << Trace::indent(' ') << TRACEOBJECT.mClassName.toLocal8Bit().data()
#define TRACECP  qDebug() << Trace::indent('#') << __FILE__ << ":" << __LINE__;

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
