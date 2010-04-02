/**
 * src/apps/Stripped/CDReaderThread.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declartion */
#include "CDReaderThread.hpp"

/* system headers */
#include <cdio/mmc.h>

/* Qt headers */
#include <QtGui>

/* local library headers */
#include <MySettings.hpp>

/* local headers */
#include "CDEdit.hpp"
#include "CDInfo.hpp"
#include "ConfigDialog.hpp"
#include "Encoder.hpp"


#include <Trace.hpp>


#define MAX_PARANOIA_FUNCTION 13


static CDReaderThread *gCDReaderThread0 = 0;

static void callback0( long inpos, ::paranoia_cb_mode_t function )
{
   gCDReaderThread0->callback( inpos, function );
}


void CDReaderThread::callback( long /*inpos*/, ::paranoia_cb_mode_t function )
{
   if( function > 11 )
   {
      mCancel = true;
      return;
   }

   if( (unsigned int)function < MAX_PARANOIA_FUNCTION )
   {
      mpCallbackFunction[(int)function]++;
   }
//   qDebug() << "callback:" << "inpos:" << inpos << "function:" << function << mCallbackFunction[function];
}


CDReaderThread::CDReaderThread( QObject *parent )
: QThread( parent )
, mpCdIo( 0 )
, mpDrive( 0 )
, mpParanoia( 0 )
, mpCDInfo( 0 )
, mpCDEdit( 0 )
, mpEncoder( 0 )
, mpCallbackFunction( new unsigned long[ MAX_PARANOIA_FUNCTION ] )
, mCancel( false )
, mDevice()
, mDevices()
{
}


CDReaderThread::~CDReaderThread()
{
   if( mpCallbackFunction )
   {
      delete[] mpCallbackFunction;
   }
}


void CDReaderThread::setup( CDInfo *info, CDEdit *edit,
                            Encoder *encoder, const QString &device )
{
   gCDReaderThread0  = this;
   mpCDInfo          = info;
   mpCDEdit          = edit;
   mpEncoder         = encoder;
   mDevice           = device;
}


void CDReaderThread::startGetDevices()
{
   mMode = modeGetDevices;
   start();
}


void CDReaderThread::startReadToc()
{
   mMode = modeReadToc;
   start();
}


void CDReaderThread::startReadCDText()
{
   mMode = modeReadCDText;
   start();
}


void CDReaderThread::startReadAudioData()
{
   mMode = modeReadAudioData;
   start();
}


void CDReaderThread::startEject()
{
   mMode = modeEject;
   start();
}


void CDReaderThread::run()
{
   switch( mMode )
   {
      case modeGetDevices:
         emit message( tr("Gettings devices.") );
         runGetDevices();
         break;
      case modeReadToc:
         emit message( tr("Reading TOC.") );
         runReadToc();
         break;
      case modeReadCDText:
         emit message( tr("Reading CD-Text.") );
         runReadCDText();
         break;
      case modeReadAudioData:
         emit message( tr("Reading audio data.") );
         runReadAudioData();
         break;
      case modeEject:
         emit message( tr("Ejecting disc.") );
         runEject();
         break;
      default:
         break;
   }
   mMode = modeUndefined;
   QTimer::singleShot( 5000, this, SIGNAL(message()) );
}


void CDReaderThread::runGetDevices()
{
   char **drives = 0;
   char **drive = 0;

   mDevices.clear();

   emit stateScan();
   if( ::cdio_init() )
   {
      drives = ::cdio_get_devices( DRIVER_DEVICE );
      if( drives )
      {
         for( drive = drives; *drive; drive++ )
         {
            mDevices.append( QString(*drive) );
         }
      }
      ::cdio_free_device_list( drives );
   }

   emit foundDevices( mDevices );
   if( mDevices.count() )
   {
      emit stateNoDisc();
   }
   else
   {
      emit message( tr("No drive found.") );
   }
}


void CDReaderThread::runReadToc()
{
   mCancel = false;
   mpCdIo  = ::cdio_open( mDevice.toLocal8Bit(), DRIVER_UNKNOWN );
   mpDrive = ::cdio_cddap_identify_cdio( mpCdIo, CDDA_MESSAGE_PRINTIT, NULL );

   emit stateScan();

   int retval = ::cdio_cddap_open(mpDrive);
   switch( retval )
   {
      case 0:
         break;
      case -2:
      case -3:
      case -4:
      case -5:
         emit stateNoDisc();
         emit message( tr("Unable to open disc. Is there an audio CD in the drive?") );
         return;
      case -6:
         emit stateNoDisc();
         emit message( tr("cdparanoia could not find a way to read audio from this drive." ) );
         return;
      default:
         emit stateNoDisc();
         emit message( tr("Unable to open disc.") );
         return;
   }

   mpCDInfo->setDisc( mpDrive->tracks,
                      ::cdio_cddap_disc_firstsector( mpDrive ),
                      ::cdio_get_track_last_lsn( mpCdIo, mpDrive->tracks ) );

   for( int i = 1; i < 100; i++ )
   {
      if( i <= mpDrive->tracks )
      {
         mpCDInfo->setTrack( i,
                             ::cdio_cddap_track_firstsector( mpDrive, i ),
                             ::cdio_cddap_track_lastsector( mpDrive, i ),
                             ::cdio_cddap_track_audiop( mpDrive, i ) != 0,
                             ::cdio_cddap_track_copyp( mpDrive, i ) != 0,
                             ::cdio_cddap_track_preemp( mpDrive, i ) != 0,
                             ::cdio_cddap_track_channels( mpDrive, i ) == 2 );
      }
      else
      {
         mpCDInfo->setTrack( i, 0, 0, false, false, false, false ) ;
      }
   }

   if( mpDrive->tracks )
   {
      mpCDInfo->calcCddbDiscID();
      emit stateDisc();
      emit gotToc();
   }
   else
   {
      mpCDInfo->clear();
      emit stateNoDisc();
   }
}


void CDReaderThread::runReadCDText()
{
   cdtext_t *cdtext;
   track_t track, first, last;

   if( !mpCdIo )
   {
      return;
   }

   emit stateScan();

   first = cdio_get_first_track_num( mpCdIo );
   last  = cdio_get_last_track_num( mpCdIo );

   cdtext = cdio_get_cdtext( mpCdIo, 0 );
   mpCDInfo->setArtist( -1, QString::fromUtf8( cdtext_get_const( CDTEXT_PERFORMER, cdtext ) ) );
   mpCDInfo->setTitle( -1, QString::fromUtf8( cdtext_get_const( CDTEXT_TITLE, cdtext ) ) );
   mpCDInfo->setGenre( QString::fromUtf8( cdtext_get_const( CDTEXT_GENRE, cdtext ) ) );
   cdtext_destroy( cdtext );

   for( track = first; track <= last; track++ )
   {
      cdtext = cdio_get_cdtext( mpCdIo, track );
      mpCDInfo->setArtist( track, QString::fromUtf8( cdtext_get_const( CDTEXT_PERFORMER, cdtext ) ) );
      mpCDInfo->setTitle( track, QString::fromUtf8( cdtext_get_const( CDTEXT_TITLE, cdtext ) ) );
      cdtext_destroy( cdtext );
   }
   emit gotToc();
   emit stateDisc();
}


void CDReaderThread::runReadAudioData()
{
   int percent = 0;
   int lastPercent = 0;
   int i = 0;
   int sector = 0;
   char *buffer = 0;
   mCancel = false;
   QString createPattern( MySettings().VALUE_CREATEPATTERN );
   QString artist, title, albumartist, albumtitle, genre;
   bool dorip, doenqueue;
   int year;

   if( !mpCdIo || !mpDrive )
   {
      return;
   }

   for( i = 0; i < MAX_PARANOIA_FUNCTION; i++ )
   {
      mpCallbackFunction[i] = 0;
   }

   emit stateRip();

   mpParanoia = ::cdio_paranoia_init( mpDrive );
   ::cdio_paranoia_modeset( mpParanoia, PARANOIA_MODE_FULL^PARANOIA_MODE_NEVERSKIP );

   emit setTrackDisabled( -1, true );
   for( i = 0; i < 100; i++ )
   {
      mpCDEdit->trackInfo( i, &dorip, &doenqueue, &artist, &title,
                           &albumartist, &albumtitle, &genre, &year );
      emit setTrackDisabled( i, true );
      emit ensureVisible( i );
      if( !dorip )
      {
         continue;
      }

      TagList tagList;
      tagList.set( "ALBUMARTIST", albumartist );
      tagList.set( "ALBUM",       albumtitle  );
      tagList.set( "ARTIST",      artist      );
      tagList.set( "TITLE",       title       );
      tagList.set( "TRACKNUMBER", QString::number(i) );
      tagList.set( "GENRE",       genre );
      if( year > 0 )
      {
         tagList.set( "DATE",        QString::number(year) );
      }

      int firstSector = mpCDInfo->firstSector( i );
      int lastSector  = mpCDInfo->lastSector( i );
      QString fileName( tagList.fileName( createPattern ) );

      emit message( fileName.mid( fileName.lastIndexOf('/')+1 ) );
      /* remove ALBUMARTIST that was only used for filename creation */
      tagList.set( "ALBUMARTIST" );
      mpEncoder->setTags( tagList );
      mpEncoder->initialize( fileName );
      connect( this, SIGNAL(encodeThis(const QByteArray &)),
               mpEncoder, SLOT(encodeCDAudio(const QByteArray &)) );
      connect( this, SIGNAL(encodeDone()),
               mpEncoder, SLOT(quit()) );
      connect( mpEncoder, SIGNAL(encodingFail()),
               this, SLOT(cancel()) );
      mpEncoder->start();

      ::cdio_paranoia_seek( mpParanoia, firstSector, SEEK_SET );
      percent = 0;
      lastPercent = 0;
      for( sector = firstSector; sector <= lastSector; sector++ )
      {
#if 0
printf("reading: %02d:%02d.%02d\r", sector/75/60, (sector/75)%60, sector%75);
fflush(stdout);
#endif
         percent = (sector - firstSector) * 100 / (lastSector - firstSector);
         if( percent != lastPercent )
         {
            emit progress( percent );
            lastPercent = percent;
         }
         buffer = (char*)::cdio_paranoia_read( mpParanoia, callback0 );
         if( !buffer )
         {
            mCancel = true;
         }
         if( mCancel )
         {
            break;
         }
         emit encodeThis( QByteArray(buffer, CDIO_CD_FRAMESIZE_RAW/*2352*/) );
         if( mCancel )
         {
            break;
         }
      }
#if 0
printf("\n");
#endif
      emit encodeDone();
      disconnect( this, SIGNAL(encodeThis(const QByteArray &)),
                  mpEncoder, SLOT(encodeCDAudio(const QByteArray &)) );
      disconnect( this, SIGNAL(encodeDone()),
                  mpEncoder, SLOT(quit()) );
      disconnect( mpEncoder, SIGNAL(encodingFail()),
                  this, SLOT(cancel()) );

      mpEncoder->finalize( doenqueue, mCancel );
      if( mCancel )
      {
         break;
      }
   }

   if( buffer )
   {
      ::cdio_paranoia_free( mpParanoia );
   }
   emit message( tr("Audio extraction completed.") );
   emit progress( 0 );
   for( i = 0; i < 100; i++ )
   {
      emit setTrackDisabled( i, false );
   }
   emit setTrackDisabled( -1, false );
   emit stateDisc();
   if( MySettings().VALUE_AUTOEJECT && !mCancel )
   {
      runEject();
   }
}


void CDReaderThread::runEject()
{
   driver_return_code_t retval;
   mpCdIo = ::cdio_open( mDevice.toLocal8Bit(), DRIVER_UNKNOWN );
   /* first try mmc command */
   retval = ::mmc_eject_media( mpCdIo );
   if( retval < 0 )
   {
      /* if it didn't work, try generic command */
      retval = ::cdio_eject_media( &mpCdIo );
   }
#ifndef Q_OS_WIN32
   if( retval < 0 )
   {
      /* on my system both of the above don't work, so use this a final
       * attempt */
      if( QProcess::execute("eject") )
      {
         emit message( tr("Eject failed.") );
      }
   }
#endif
   QTimer::singleShot( 1000, this, SIGNAL(message()) );
   emit stateNoDisc();
}


void CDReaderThread::cancel()
{
   mCancel = true;
   /* TODO: set timer to stop thread */
}