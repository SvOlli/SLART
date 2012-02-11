/*
 * src/app/Rubberbandman/Console.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "Console.hpp"

/* system headers */
#include <cstdio>

/* Qt headers */

/* local library headers */

/* local headers */


Console::Console( const QString &checkedText,
                  const QString &processedText,
                  QObject *parent )
: QObject( parent )
, mStdOut( ::stdout )
, mCheckedText( checkedText )
, mProcessedText( processedText )
{
}


Console::~Console()
{
   mStdOut << tr( "Done." ) << "\n";
   mStdOut.flush();
}


void Console::message( const QString &msg )
{
   mStdOut << msg << "\n";
   mStdOut.flush();
}


void Console::handleProgress( int checked, int processed )
{
   mStdOut << "\r"
           << QString("%1 %2, %3 %4. ").arg( checked ).arg( mCheckedText )
                                       .arg( processed ).arg( mProcessedText );
   mStdOut.flush();
}
