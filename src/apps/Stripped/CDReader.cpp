/*
 * src/apps/Stripped/CDReader.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "CDReader.hpp"

/* system headers */

/* Qt headers */
#include <QApplication>
#include <QGridLayout>
#include <QLabel>
#include <QMessageBox>
#include <QProgressBar>

/* local library headers */
#include <MagicEncoderInterface.hpp>
#include <Settings.hpp>

/* local headers */
#include "CDInfo.hpp"
#include "CDEdit.hpp"
#include "CDReaderThread.hpp"
#include "ParanoiaStatus.hpp"


#include <Trace.hpp>


//#define TRACETEXT(x) TRACEMSG << cdtext_field2str( x ) << cdtext_get( x, cdtext )

CDReader::CDReader( CDInfo *info, CDEdit *edit, QWidget *parent )
: QWidget( parent )
, mpCDReaderThread( new CDReaderThread() )
, mpCDInfo( info )
, mpCDEdit( edit )
, mpProgressBar( new QProgressBar( this ) )
, mpRippingSpeed( new QLabel( this ) )
, mEncoders()
, mDevice()
{
   mpProgressBar->setRange( 0, 100 );
   mpProgressBar->setAlignment( Qt::AlignCenter );

   QGridLayout *mainLayout = new QGridLayout( this );
   mainLayout->setContentsMargins( 0, 0, 0, 0 );

   mainLayout->addWidget( mpRippingSpeed, 1, 0 );
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
   connect( mpCDReaderThread, SIGNAL(gotData()),
            this, SIGNAL(gotData()) );
   connect( mpCDReaderThread, SIGNAL(noEject()),
            this, SLOT(noEject()) );
   connect( mpCDReaderThread, SIGNAL(foundDevices(QStringList)),
            this, SIGNAL(foundDevices(QStringList)) );
   connect( mpCDReaderThread, SIGNAL(message(QString)),
            this, SIGNAL(message(QString)) );
   connect( mpCDReaderThread, SIGNAL(setTrackDisabled(int,bool)),
            this, SIGNAL(setTrackDisabled(int,bool)) );
   connect( mpCDReaderThread, SIGNAL(ensureVisible(int)),
            this, SIGNAL(ensureVisible(int)) );
   connect( mpCDReaderThread, SIGNAL(progress(int,int)),
            this, SLOT(updateProgress(int,int)) );
   connect( mpCDReaderThread, SIGNAL(errors(int,unsigned int,const unsigned long*)),
            this, SIGNAL(errors(int,unsigned int,const unsigned long*)) );

   updateProgress( 0, 0 );
   setLayout( mainLayout );
}


CDReader::~CDReader()
{
   if( mpCDReaderThread->isRunning() )
   {
      mpCDReaderThread->quit();
      mpCDReaderThread->wait();
   }
   delete mpCDReaderThread;
}


void CDReader::setEncoders( const MagicEncoderList &encoders )
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


void CDReader::noEject()
{
   QMessageBox::warning( this, tr("%1: Read Error").arg( QApplication::applicationName() ),
                         tr("There has been an error during reading the disc.\n"
                            "Therefore the disc has not been ejected.") );
}


void CDReader::updateProgress( int percent, int speed )
{
   double showSpeed( speed );
   showSpeed /= 75;
   mpProgressBar->setValue( percent );
   mpRippingSpeed->setText( tr("Ripping Speed: %1x").arg(showSpeed,3,'g',2) );
}
