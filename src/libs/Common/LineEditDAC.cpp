/**
 * src/MyLineEdit.cpp
 * written by Sven Oliver Moll
 */

/* class declaration */
#include "LineEditDAC.hpp"

/* system headers */

/* Qt headers */
#include <QCompleter>
#include <QTimer>

/* local library headers */

/* local headers */


LineEditDAC::LineEditDAC( QWidget *parent )
: QLineEdit( parent )
, mpTimer( new QTimer( this ) )
, mpCompleter( 0 )
, mDelay( 100 )
{
   mpTimer->setSingleShot( true );

   connect( mpTimer, SIGNAL(timeout()),
            this, SLOT(setCompleter()) );
}


LineEditDAC::~LineEditDAC()
{
   if( mpCompleter )
   {
      delete mpCompleter;
      mpCompleter = 0;
   }
}


void LineEditDAC::setCompleter( QCompleter *completer )
{
   mpCompleter = completer;
}


void LineEditDAC::setText( const QString &text )
{
   if( mDelay > 0 )
   {
      QLineEdit::setCompleter( 0 );
      QLineEdit::setText( text );
      mpTimer->start( mDelay );
   }
   else
   {
      QLineEdit::setText( text );
   }
}


int LineEditDAC::delay() const
{
   return mDelay;
}


void LineEditDAC::setDelay( int msec )
{
   mDelay = msec;
}


void LineEditDAC::setCompleter()
{
   QLineEdit::setCompleter( mpCompleter );
}
