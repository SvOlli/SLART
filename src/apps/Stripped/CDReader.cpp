/**
 * src/apps/Stripped/CDReader.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "CDReader.hpp"

/* system headers */

/* Qt headers */
#include <QtGui>

/* local library headers */
#include <MySettings.hpp>
#include <ConfigDialog.hpp>
#include <TagList.hpp>

/* local headers */
#include "CDInfo.hpp"
#include "CDEdit.hpp"
#include "CDReaderThread.hpp"
#include "Encoder.hpp"


#include <Trace.hpp>


//#define TRACETEXT(x) TRACEMSG << cdtext_field2str( x ) << cdtext_get( x, cdtext )

CDReader::CDReader( CDInfo *info, CDEdit *edit, QWidget *parent )
: QWidget( parent )
, mpCDReaderThread( new CDReaderThread( this ) )
, mpCDInfo( info )
, mpCDEdit( edit )
, mpProgressBar( new QProgressBar( this ) )
, mEncoders()
, mDevice()
{
   mpProgressBar->setRange( 0, 100 );

   QGridLayout *mainLayout = new QGridLayout( this );
#if QT_VERSION < 0x040300
   mainLayout->setMargin( 0 );
#else
   mainLayout->setContentsMargins( 0, 0, 0, 0 );
#endif

   mainLayout->addWidget( mpProgressBar, 0, 0 );

   connect( mpCDReaderThread, SIGNAL(stateNoDisc()),
            this, SIGNAL(stateNoDisc()) );
   connect( mpCDReaderThread, SIGNAL(stateDisc()),
            this, SIGNAL(stateDisc()) );
   connect( mpCDReaderThread, SIGNAL(stateScan()),
            this, SIGNAL(stateScan()) );
   connect( mpCDReaderThread, SIGNAL(stateRip()),
            this, SIGNAL(stateRip()) );
   connect( mpCDReaderThread, SIGNAL(gotToc()),
            this, SIGNAL(gotToc()) );
   connect( mpCDReaderThread, SIGNAL(foundDevices(const QStringList &)),
            this, SIGNAL(foundDevices(const QStringList &)) );
   connect( mpCDReaderThread, SIGNAL(message(const QString &)),
            this, SIGNAL(message(const QString &)) );
   connect( mpCDReaderThread, SIGNAL(setTrackDisabled(int,bool)),
            this, SIGNAL(setTrackDisabled(int,bool)) );
   connect( mpCDReaderThread, SIGNAL(ensureVisible(int)),
            this, SIGNAL(ensureVisible(int)) );
   connect( mpCDReaderThread, SIGNAL(progress(int)),
            mpProgressBar, SLOT(setValue(int)) );

   setLayout( mainLayout );
}


CDReader::~CDReader()
{
}


void CDReader::setEncoders( const QList<Encoder*> &encoders )
{
   mEncoders = encoders;
}


void CDReader::setDevice( const QString &device )
{
   mDevice = device;
}


void CDReader::getDevices()
{
   mpCDReaderThread->startGetDevices();
}


void CDReader::readToc()
{
   mpCDInfo->clear();
   mpCDEdit->clear();
   mpCDReaderThread->setup( mpCDInfo, mpCDEdit, mEncoders, mDevice );
   mpCDReaderThread->startReadToc();
}


void CDReader::readCDText()
{
   mpCDReaderThread->setup( mpCDInfo, mpCDEdit, mEncoders, mDevice );
   mpCDReaderThread->startReadCDText();
}


void CDReader::readTracks()
{
   mpCDReaderThread->setup( mpCDInfo, mpCDEdit, mEncoders, mDevice );
   mpCDReaderThread->startReadAudioData();
}


void CDReader::eject()
{
   mpCDReaderThread->setup( mpCDInfo, mpCDEdit, mEncoders, mDevice );
   mpCDReaderThread->startEject();
}


void CDReader::cancel()
{
   mpCDReaderThread->cancel();
}
