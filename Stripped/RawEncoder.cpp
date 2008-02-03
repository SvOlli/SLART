/**
 * RawEncoder.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#include "RawEncoder.hpp"

#include "Trace.hpp"

extern "C"
{
#include <unistd.h>
};

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


void RawEncoder::finalize()
{
   Encoder::finalize();
}


void RawEncoder::setTag( const QString &/*tag*/, const QString &/*value*/ )
{
}


void RawEncoder::encodeCDAudio( const char* data, int size )
{
   ::write( mFD, data, size );
}
