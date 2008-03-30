/**
 * CDReader.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#include "CDReader.hpp"
#include "CDToc.hpp"
#include "CDEdit.hpp"

#include <QtGui>
#include "Trace.hpp"
#include "Encoder.hpp"
#include "MySettings.hpp"
#include "TagList.hpp"


static CDReader *gCDReader0;


static void callback0( long inpos, ::paranoia_cb_mode_t function )
{
   gCDReader0->callback( inpos, function );
}


void CDReader::callback( long /*inpos*/, ::paranoia_cb_mode_t function )
{
   mCallbackFunction[(int)function]++;
//   qDebug() << "callback:" << "inpos:" << inpos << "function:" << function << mCallbackFunction[function];
}


CDReader::CDReader( CDToc *toc, CDEdit *edit, QWidget *parent , Qt::WindowFlags flags )
: QWidget( parent, flags )
, mpCdIo( 0 )
, mpDrive( 0 )
, mpParanoia( 0 )
, mpToc( toc )
, mpCDEdit( edit )
, mpMessage( new QLabel( this ) )
, mpProgress( new QProgressBar( this ) )
{
   gCDReader0 = this;
   
   QGridLayout *mainLayout = new QGridLayout( this );
#if QT_VERSION < 0x040300
   mainLayout->setMargin( 0 );
#else
   mainLayout->setContentsMargins( 0, 0, 0, 0 );
#endif

   mainLayout->setColumnStretch ( 0, 1 );
   mainLayout->setColumnStretch ( 1, 0 );
   mainLayout->addWidget( mpMessage, 0, 0, 1, 2 );
   mainLayout->addWidget( mpProgress, 1, 0, 1, 2 );
   
   setLayout( mainLayout );
   if( !::cdio_init() )
   {
      fprintf( stderr, "cdio_init() failed\n" );
   }
}


void CDReader::setDevice( const QString &device )
{
   mDevice = device;
}


void CDReader::getDevices( QComboBox *comboBox )
{
   char **drives,**d;
   
   comboBox->clear();
   
   drives = ::cdio_get_devices( DRIVER_UNKNOWN );
   if( !drives ) return;
   
   for( d = drives; *d; d++ )
   {
      comboBox->addItem( QString(*d) );
   }
}


void CDReader::readToc()
{
   mCancel = false;
   emit starting();
   mpCDEdit->clear();
   QCoreApplication::processEvents();
   QCoreApplication::processEvents();
   
//   mpDrive = ::cdio_cddap_find_a_cdrom( CDDA_MESSAGE_PRINTIT, NULL );
   mpCdIo  = ::cdio_open( mDevice.toLocal8Bit(), DRIVER_UNKNOWN );
   mpDrive = ::cdio_cddap_identify_cdio( mpCdIo, CDDA_MESSAGE_PRINTIT, NULL );
   
   switch( ::cdio_cddap_open(mpDrive) )
   {
      case 0:
         break;
      case -2:case -3:case -4:case -5:
         //puts("\nUnable to open disc.  Is there an audio CD in the drive?");
         exit(1);
      case -6:
         //puts("\ncdparanoia could not find a way to read audio from this drive."
         exit(1);
      default:
         //puts("\nUnable to open disc.");
         exit(1);
   }

   mpToc->setDisc( mpDrive->tracks,
                   ::cdio_cddap_disc_firstsector( mpDrive ), ::cdio_get_track_last_lsn( mpCdIo, mpDrive->tracks ) );
   
   for( int i = 1; i < 100; i++ )
   {
      if( i <= mpDrive->tracks )
      {
         mpToc->setEntry( i, ::cdio_cddap_track_firstsector( mpDrive, i ), ::cdio_cddap_track_lastsector( mpDrive, i ),
                         ::cdio_cddap_track_audiop( mpDrive, i ) != 0, ::cdio_cddap_track_copyp( mpDrive, i ) != 0,
                         ::cdio_cddap_track_preemp( mpDrive, i ) != 0, ::cdio_cddap_track_channels( mpDrive, i ) == 2 );
      }
      else
      {
         mpToc->setEntry( i, 0, 0, false, false, false, false ) ;
      }
   }
   
   mpToc->calcCddbDiscID();
   
   mpCDEdit->update();
   
   emit stopping();
}

#define TRACETEXT(x) TRACEMSG << cdtext_field2str( x ) << cdtext_get( x, cdtext )

void CDReader::readCDText()
{
   emit starting();
   QCoreApplication::processEvents();
   QCoreApplication::processEvents();
   
   cdtext_t *cdtext;
   track_t track, first, last;
   
   first = cdio_get_first_track_num( mpCdIo );
   last  = cdio_get_last_track_num( mpCdIo );
   
   cdtext = cdio_get_cdtext( mpCdIo, 0 );
   mpCDEdit->updateCDText( 0, 
                         QString( cdtext_get( CDTEXT_PERFORMER, cdtext ) ),
                         QString( cdtext_get( CDTEXT_TITLE, cdtext ) ) );
   cdtext_destroy( cdtext );
   
   for( track = first; track <= last; track++ )
   {
      cdtext = cdio_get_cdtext( mpCdIo, track );
      mpCDEdit->updateCDText( track, 
                            QString( cdtext_get( CDTEXT_PERFORMER, cdtext ) ),
                            QString( cdtext_get( CDTEXT_TITLE, cdtext ) ) );
      cdtext_destroy( cdtext );
   }
   
   emit stopping();
}


void CDReader::readTracks()
{
   int i;
   int sector;
   char *buffer = 0;
   mCancel = false;
   QString createPattern( MySettings().value("CreatePattern",
                          "|$ALBUMARTIST|/|$ALBUM|/(|#2TRACKNUMBER|)|$ARTIST| - |$TITLE|").toString() );
   
   QString artist, title, albumartist, albumtitle, genre;
   bool dorip, doenqueue;
   int year;
   
   for( i = 0; i < 13; i++ )
   {
      mCallbackFunction[i] = 0;
   }
   
#if 0
   for( i = 1; i < 53; i++ )
   {
TRACEMSG << "speed:" << i << ::cdio_cddap_speed_set( mpDrive, i );
   }
#endif
   emit starting();
   QCoreApplication::processEvents();
   QCoreApplication::processEvents();
   
   mpParanoia = ::cdio_paranoia_init( mpDrive );
   ::cdio_paranoia_modeset( mpParanoia, PARANOIA_MODE_FULL^PARANOIA_MODE_NEVERSKIP );

   for( i = 0; i < 100; i++ )
   {
      mpCDEdit->trackInfo( i, &dorip, &doenqueue, &artist, &title, 
                           &albumartist, &albumtitle, &genre, &year );
      mpCDEdit->setTrackDisabled( i, true );
      mpCDEdit->ensureVisible( i );
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
      tagList.set( "DATE",        QString::number(year) );
      
      int firstSector = mpToc->firstSector( i );
      int lastSector  = mpToc->lastSector( i );
      QString fileName( tagList.fileName( createPattern ) );
      
      mpEncoder->initialize( fileName );
      
      mpMessage->setText( fileName.mid( fileName.lastIndexOf('/')+1 ) );
      tagList.set( "ALBUMARTIST" );
      mpEncoder->setTags( tagList );
      ::cdio_paranoia_seek( mpParanoia, firstSector, SEEK_SET );
      mpProgress->setRange( firstSector, lastSector );
      for( sector = firstSector; sector <= lastSector; sector++ )
      {
         buffer = (char*)::cdio_paranoia_read( mpParanoia, callback0 );
         mpEncoder->encodeCDAudio( buffer, 2352 );
         mpProgress->setValue( sector );
         QCoreApplication::processEvents();
         if( mCancel ) break;
      }
      
      mpEncoder->finalize( doenqueue );
      if( mCancel ) break;
   }
   
   if( buffer )
   {
      ::cdio_paranoia_free( mpParanoia );
   }
   mpMessage->setText( tr("Done.") );
   mpProgress->setValue( 0 );
   mpProgress->setRange( 0, 1 );
   if( !mCancel )
   {
      eject();
   }
   for( i = 0; i < 100; i++ )
   {
      mpCDEdit->setTrackDisabled( i, false );
   }   
   emit stopping();
}


void CDReader::updateCDDisplay()
{
}


void CDReader::setEncoder( Encoder *encoder )
{
   mpEncoder = encoder;
}


void CDReader::eject()
{
   mpCdIo  = ::cdio_open( mDevice.toLocal8Bit(), DRIVER_UNKNOWN );
   ::cdio_eject_media( &mpCdIo );
}


void CDReader::cancel()
{
   mCancel = true;
}
