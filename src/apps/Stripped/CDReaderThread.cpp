/*
 * src/apps/Stripped/CDReaderThread.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declartion */
#include "CDReaderThread.hpp"

/* system headers */
extern "C" {
#include <cdio/mmc.h>
}

/* Qt headers */
#include <QtGui>
#include <QElapsedTimer>

/* local library headers */
#include <MagicEncoderProxy.hpp>
#include <Satellite.hpp>
#include <Settings.hpp>

/* local headers */
#include "CDEdit.hpp"
#include "CDInfo.hpp"


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
      ::fprintf( stderr, "function:%d\n", func );
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
   bool useLatin1 = Settings::value( Settings::StrippedCDTextLatin1 );

   if( !mpCdIo )
   {
      return;
   }

   emit stateScan();

   first = cdio_get_first_track_num( mpCdIo );
   last  = cdio_get_last_track_num( mpCdIo );

#if CDIO_API_VERSION <= 5
   cdtext = cdio_get_cdtext( mpCdIo, 0 );
   if( cdtext )
   {
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
   }

   for( track = first; track <= last; track++ )
   {
      cdtext = cdio_get_cdtext( mpCdIo, track );
      if( cdtext )
      {
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
   }
#else
   cdtext = cdio_get_cdtext( mpCdIo );
   if( cdtext )
   {
      if( useLatin1 )
      {
         mpCDInfo->setArtist( -1, QString::fromLatin1( cdtext_get_const( cdtext, CDTEXT_FIELD_PERFORMER, 0 ) ) );
         mpCDInfo->setTitle( -1, QString::fromLatin1( cdtext_get_const( cdtext, CDTEXT_FIELD_TITLE, 0 ) ) );
         mpCDInfo->setGenre( QString::fromLatin1( cdtext_get_const( cdtext, CDTEXT_FIELD_GENRE, 0 ) ) );
         for( track = first; track <= last; track++ )
         {
            mpCDInfo->setArtist( track, QString::fromLatin1( cdtext_get_const( cdtext, CDTEXT_FIELD_PERFORMER, track ) ) );
            mpCDInfo->setTitle( track, QString::fromLatin1( cdtext_get_const( cdtext, CDTEXT_FIELD_TITLE, track ) ) );
         }
      } else {
         mpCDInfo->setTitle( -1, QString::fromUtf8( cdtext_get_const( cdtext, CDTEXT_FIELD_TITLE, 0 ) ) );
         mpCDInfo->setArtist( -1, QString::fromUtf8( cdtext_get_const( cdtext, CDTEXT_FIELD_PERFORMER, 0 ) ) );
         mpCDInfo->setGenre( QString::fromUtf8( cdtext_get_const( cdtext, CDTEXT_FIELD_GENRE, 0 ) ) );
         for( track = first; track <= last; track++ )
         {
            mpCDInfo->setArtist( track, QString::fromUtf8( cdtext_get_const( cdtext, CDTEXT_FIELD_PERFORMER, track ) ) );
            mpCDInfo->setTitle( track, QString::fromUtf8( cdtext_get_const( cdtext, CDTEXT_FIELD_TITLE, track ) ) );
         }
      }
      cdtext_destroy( cdtext );
   }
#endif
   emit stateDisc();
   emit gotData();
}


void CDReaderThread::runReadAudioData()
{
   int percent = 0;
   int lastPercent = 0;
   int track = 0;
   int sector = 0;
   int trackSectorsRead = 0;
   int totalSectorsRead = 0;
   char *buffer = 0;
   mCancel = false;
   QString artist;
   QString title;
   QString albumartist;
   QString albumtitle;
   QString genre;
   bool dorip;
   bool doenqueue;
   int year;
   QString createPattern = Settings::value( Settings::StrippedCreatePattern );
   bool autoEnqueue      = Settings::value( Settings::StrippedAutoEnqueue );
   QElapsedTimer trackTime;
   QElapsedTimer totalTime;

   if( !mpCdIo || !mpDrive )
   {
      return;
   }

   emit stateRip();

//   qDebug() << "cdio_set_speed(): " << ::cdio_set_speed( mpCdIo, 1 );
   emit setTrackDisabled( -1, true );
   mDiscHasErrors = false;
   totalTime.start();
   for( track = 0; track < 100; track++ )
   {
      mpParanoia = ::cdio_paranoia_init( mpDrive );
      ::cdio_paranoia_modeset( mpParanoia, PARANOIA_MODE_FULL^PARANOIA_MODE_NEVERSKIP );

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

      TagMap tagMap;
      tagMap.insert( "ALBUMARTIST", albumartist );
      tagMap.insert( "ALBUM",       albumtitle  );
      tagMap.insert( "ARTIST",      artist      );
      tagMap.insert( "TITLE",       title       );
      tagMap.insert( "TRACKNUMBER", QString::number(track) );
      tagMap.insert( "GENRE",       genre );
      if( year > 0 )
      {
         tagMap.insert( "DATE",        QString::number(year) );
      }

      int firstSector = mpCDInfo->firstSector( track );
      int lastSector  = mpCDInfo->lastSector( track );
      QString fileName( tagMap.fileName( createPattern ) );

      emit message( fileName.mid( fileName.lastIndexOf('/')+1 ) );
      /* remove ALBUMARTIST that was only used for filename creation */
      tagMap.remove( "ALBUMARTIST" );
      bool setEnqueue = true;
      foreach( MagicEncoderProxy *encoder, mEncoders )
      {
         if( encoder->useEncoder() )
         {
            QThread *thread = encoder->workerThread();
            thread->start();
            encoder->setTags( tagMap );
            encoder->initialize( fileName );
            connect( this, SIGNAL(encodeThis(QByteArray)),
                     thread, SLOT(encodeCDAudio(QByteArray)) );
            connect( this, SIGNAL(encodeDone()),
                     thread, SLOT(quit()) );
            connect( thread, SIGNAL(encodingFail()),
                     this, SLOT(cancel()) );
            encoder->setEnqueue( setEnqueue );
            setEnqueue = false;
         }
      }

      ::cdio_paranoia_seek( mpParanoia, firstSector, SEEK_SET );
      lastPercent = 0;
      trackTime.start();
      trackSectorsRead = 0;
      for( sector = firstSector; sector <= lastSector; sector++ )
      {
#if 0
printf("reading: %02d:%02d.%02d\r", sector/75/60, (sector/75)%60, sector%75);
fflush(stdout);
#endif
         percent = (sector - firstSector) * 100 / (lastSector - firstSector);
         trackSectorsRead++;
         if( percent != lastPercent )
         {
            emit errors( track, MAX_PARANOIA_FUNCTION, mpCallbackFunction );
            emit progress( percent, trackSectorsRead * 1000 / trackTime.elapsed() );
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
      totalSectorsRead += trackSectorsRead;
#if 0
printf("\n");
#endif
      emit encodeDone();
      foreach( MagicEncoderProxy *encoder, mEncoders )
      {
         if( encoder->useEncoder() )
         {
            QThread *thread = encoder->workerThread();
            disconnect( this, SIGNAL(encodeThis(QByteArray)),
                        thread, SLOT(encodeCDAudio(QByteArray)) );
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
      if( buffer )
      {
         ::cdio_paranoia_free( mpParanoia );
      }
      if( mCancel )
      {
         break;
      }
   }

   emit message( tr("Audio extraction completed.") );
   emit progress( 0, totalSectorsRead * 1000 / totalTime.elapsed() );
   for( track = 0; track < 100; track++ )
   {
      emit setTrackDisabled( track, false );
   }
   emit setTrackDisabled( -1, false );
   emit stateDisc();
   if( Settings::value( Settings::StrippedAutoEject ) && !mCancel )
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
   /* first try generic command */
   retval = ::cdio_eject_media( &mpCdIo );
#ifndef Q_OS_WIN32
   if( retval < 0 )
   {
      /* on my system both of the above don't work, so use this a final
       * attempt */
      if( QProcess::execute("eject", QStringList()) )
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
   /* \todo: set timer to stop thread */
}
