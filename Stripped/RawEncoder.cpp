/**
 * RawEncoder.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#include "RawEncoder.hpp"
#include "TagList.hpp"

#include "Trace.hpp"

extern "C"
{
#include <unistd.h>
}

#include <QtGui>

RawEncoder::RawEncoder( QWidget *parent )
: Encoder( parent, tr("raw") )
{
   QHBoxLayout *mainLayout = new QHBoxLayout( this );
   QLabel      *label      = new QLabel( tr("no config"), this );

#if QT_VERSION < 0x040300
   mainLayout->setMargin( 0 );
#else
   mainLayout->setContentsMargins( 0, 0, 0, 0 );
#endif
   mainLayout->addWidget( label );

   setLayout( mainLayout );
   
}

RawEncoder::~RawEncoder()
{
}


void RawEncoder::initialize( const QString &fileName )
{
   Encoder::initialize( fileName, ".raw" );
}


void RawEncoder::finalize( bool enqueue, bool cancel )
{
   Encoder::finalize( enqueue, cancel );
}


void RawEncoder::setTags( const TagList &/*tagList*/ )
{
}


bool RawEncoder::encodeCDAudio( const char* data, int size )
{
   if( ::write( mFD, data, size ) < 0 )
   {
      return false;
   }
   return true;
}
