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
#include <MagicEncoderProxy.hpp>
#include <MySettings.hpp>
#include <Satellite.hpp>

/* local headers */
#include "CDEdit.hpp"
#include "CDInfo.hpp"
#include "ConfigDialog.hpp"


#include <Trace.hpp>


#define MAX_PARANOIA_FUNCTION 13


static CDReaderThread *gpCDReaderThread0 = 0;

static void callback0( long inpos, ::paranoia_cb_mode_t function )
{
   gpCDReaderThread0->callback( inpos, function );
}


void CDReaderThread::callback( long /*inpos*/, ::paranoia_cb_mode_t function )
{
   int func = (int)function;
   if( func > 12 )
   {
      printf( "function:%d\n", func );
      mCancel = true;
      return;
   }

   if( (unsigned int)func < MAX_PARANOIA_FUNCTION )
   {
      mpCallbackFunction[func]++;
      if( (func > 1) && (func != 9) )
      {
         mTrackHasErrors = true;
      }
   }
}


CDReaderThread::CDReaderThread()
: QThread( 0 )
, mpCdIo( 0 )
, mpDrive( 0 )
, mpParanoia( 0 )
, mpCDInfo( 0 )
, mpCDEdit( 0 )
, mpCallbackFunction( new unsigned long[ MAX_PARANOIA_FUNCTION ] )
, mCancel( false )
, mTrackHasErrors( false )
, mDiscHasErrors( false )
, mDevice()
, mDevices()
, mEncoders()
{
   moveToThread( this );
}


CDReaderThread::~CDReaderThread()
{
   if( mpCallbackFunction )
   {
      delete[] mpCallbackFunction;
   }
}


void CDReaderThread::setup( CDInfo *info, CDEdit *edit,
                            const MagicEncoderList &encoders, const QString &device )
{
   gpCDReaderThread0 = this;
   mpCDInfo          = info;
   mpCDEdit          = edit;
   mEncoders         = encoders;
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
         emit message( tr("Getting devices.") );
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
      emit message( tr("Insert CD to scan.") );
   }
   else
   {
      emit message( tr("No drive found.") );
   }
}


void CDReaderThread::runReadToc()
{
   mCancel = false;
   mpCdIo  = ::cdio_open( mDevice.toLocal8Bit().constData(), DRIVER_UNKNOWN );
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
   bool useLatin1 = MySettings().VALUE_CDTEXT_LATIN1;

   if( !mpCdIo )
   {
      return;
   }

   emit stateScan();

   first = cdio_get_first_track_num( mpCdIo );
   last  = cdio_get_last_track_num( mpCdIo );

   cdtext = cdio_get_cdtext( mpCdIo, 0 );
   if( useLatin1 )
   {
      mpCDInfo->setArtist( -1, QString::fromLatin1( cdtext_get_const( CDTEXT_PERFORMER, cdtext ) ) );
      mpCDInfo->setTitle( -1, QString::fromLatin1( cdtext_get_const( CDTEXT_TITLE, cdtext ) ) );
      mpCDInfo->setGenre( QString::fromLatin1( cdtext_get_const( CDTEXT_GENRE, cdtext ) ) );
   } else {
      mpCDInfo->setArtist( -1, QString::fromUtf8( cdtext_get_const( CDTEXT_PERFORMER, cdtext ) ) );
      mpCDInfo->setTitle( -1, QString::fromUtf8( cdtext_get_const( CDTEXT_TITLE, cdtext ) ) );
      mpCDInfo->setGenre( QString::fromUtf8( cdtext_get_const( CDTEXT_GENRE, cdtext ) ) );
   }
   cdtext_destroy( cdtext );

   for( track = first; track <= last; track++ )
   {
      cdtext = cdio_get_cdtext( mpCdIo, track );
      if( useLatin1 )
      {
         mpCDInfo->setArtist( track, QString::fromLatin1( cdtext_get_const( CDTEXT_PERFORMER, cdtext ) ) );
         mpCDInfo->setTitle( track, QString::fromLatin1( cdtext_get_const( CDTEXT_TITLE, cdtext ) ) );
      } else {
         mpCDInfo->setArtist( track, QString::fromUtf8( cdtext_get_const( CDTEXT_PERFORMER, cdtext ) ) );
         mpCDInfo->setTitle( track, QString::fromUtf8( cdtext_get_const( CDTEXT_TITLE, cdtext ) ) );
      }
      cdtext_destroy( cdtext );
   }
   emit gotToc();
   emit stateDisc();
}


void CDReaderThread::runReadAudioData()
{
   int percent = 0;
   int lastPercent = 0;
   int track = 0;
   int sector = 0;
   char *buffer = 0;
   mCancel = false;
   QString createPattern;
   QString artist, title, albumartist, albumtitle, genre;
   bool autoEnqueue;
   bool dorip, doenqueue;
   int year;
   {
      MySettings settings;
      createPattern = settings.VALUE_CREATEPATTERN;
      autoEnqueue   = settings.VALUE_AUTOENQUEUE;
   }


   if( !mpCdIo || !mpDrive )
   {
      return;
   }

   emit stateRip();

   mpParanoia = ::cdio_paranoia_init( mpDrive );
   ::cdio_paranoia_modeset( mpParanoia, PARANOIA_MODE_FULL^PARANOIA_MODE_NEVERSKIP );

   emit setTrackDisabled( -1, true );
   mDiscHasErrors = false;
   for( track = 0; track < 100; track++ )
   {
      mpCDEdit->trackInfo( track, &dorip, &doenqueue, &artist, &title,
                           &albumartist, &albumtitle, &genre, &year );
      emit setTrackDisabled( track, true );
      emit ensureVisible( track );
      if( !dorip )
      {
         continue;
      }
      for( int n = 0; n < MAX_PARANOIA_FUNCTION; n++ )
      {
         mpCallbackFunction[n] = 0;
      }
      mTrackHasErrors = false;

      TagList tagList;
      tagList.set( "ALBUMARTIST", albumartist );
      tagList.set( "ALBUM",       albumtitle  );
      tagList.set( "ARTIST",      artist      );
      tagList.set( "TITLE",       title       );
      tagList.set( "TRACKNUMBER", QString::number(track) );
      tagList.set( "GENRE",       genre );
      if( year > 0 )
      {
         tagList.set( "DATE",        QString::number(year) );
      }

      int firstSector = mpCDInfo->firstSector( track );
      int lastSector  = mpCDInfo->lastSector( track );
      QString fileName( tagList.fileName( createPattern ) );

      emit message( fileName.mid( fileName.lastIndexOf('/')+1 ) );
      /* remove ALBUMARTIST that was only used for filename creation */
      tagList.set( "ALBUMARTIST" );
      bool setEnqueue = true;
      foreach( MagicEncoderProxy *encoder, mEncoders )
      {
         if( encoder->useEncoder() )
         {
            QThread *thread = encoder->workerThread();
            thread->start();
            encoder->setTags( tagList );
            encoder->initialize( fileName );
            connect( this, SIGNAL(encodeThis(const QByteArray &)),
                     thread, SLOT(encodeCDAudio(const QByteArray &)) );
            connect( this, SIGNAL(encodeDone()),
                     thread, SLOT(quit()) );
            connect( thread, SIGNAL(encodingFail()),
                     this, SLOT(cancel()) );
            encoder->setEnqueue( setEnqueue );
            setEnqueue = false;
         }
      }

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
            emit errors( track, MAX_PARANOIA_FUNCTION, mpCallbackFunction );
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
      foreach( MagicEncoderProxy *encoder, mEncoders )
      {
         if( encoder->useEncoder() )
         {
            QThread *thread = encoder->workerThread();
            disconnect( this, SIGNAL(encodeThis(const QByteArray &)),
                        thread, SLOT(encodeCDAudio(const QByteArray &)) );
            disconnect( this, SIGNAL(encodeDone()),
                        thread, SLOT(quit()) );
            disconnect( thread, SIGNAL(encodingFail()),
                        this, SLOT(cancel()) );

            if( mTrackHasErrors )
            {
               mDiscHasErrors = true;
               if( autoEnqueue )
               {
                  doenqueue = true;
               }
            }
            encoder->finalize( doenqueue, mCancel );
         }
      }
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
   for( track = 0; track < 100; track++ )
   {
      emit setTrackDisabled( track, false );
   }
   emit setTrackDisabled( -1, false );
   emit stateDisc();
   if( MySettings().VALUE_AUTOEJECT && !mCancel )
   {
      if( mDiscHasErrors )
      {
         emit noEject();
      }
      else
      {
         runEject();
      }
   }
}


void CDReaderThread::runEject()
{
   driver_return_code_t retval;
   mpCdIo = ::cdio_open( mDevice.toLocal8Bit().constData(), DRIVER_UNKNOWN );
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
