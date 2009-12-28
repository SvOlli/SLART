/**
 * src/libs/Common/Trace.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#include "Trace.hpp"

volatile unsigned int Trace::mDepth = 0;

Trace::Trace( const QString &className )
: mClassName( className )
{
mDepth++;
qDebug() << QString(mDepth,'>').toLocal8Bit().data() << mClassName.toLocal8Bit().data();
}

Trace::~Trace()
{
qDebug() << QString(mDepth,'<').toLocal8Bit().data() << mClassName.toLocal8Bit().data();
mDepth--;
}

const char* Trace::indent(QChar c)
{
   return QString(mDepth,c).toLocal8Bit().data();
}
