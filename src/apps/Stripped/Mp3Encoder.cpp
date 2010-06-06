/**
 * src/apps/Stripped/Mp3Encoder.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "Mp3Encoder.hpp"

/* system headers */
#include <cstdlib>
extern "C" {
#include <lame/lame.h>
}

/* Qt headers */
#include <QtGui>

/* local library headers */
#include <MySettings.hpp>
#include <ScrollLine.hpp>
#include <TagList.hpp>

/* local headers */
#include "ConfigDialog.hpp"

#include <Trace.hpp>

Mp3Encoder::Mp3Encoder( QWidget *parent )
: Encoder( parent, tr("mp3") )
, mLame( 0 )
, mUseAbr( false )
, mUseLatin1( false )
, mpConfigWidget( new QWidget( parent ) )
, mpUseEncoder( new QCheckBox( tr("Use This Encoder"), mpConfigWidget ) )
, mpDirOverride( new QCheckBox( tr("Override Base Directory"), mpConfigWidget ) )
, mpDirectory( new ScrollLine( mpConfigWidget ) )
, mpDotButton( new QPushButton( tr("..."), mpConfigWidget ) )
, mpUseLatin1( new QCheckBox( tr("Use Latin1 instead of Utf-8"), mpConfigWidget ) )
, mMp3BufferSize( 8192 )
, mpMp3Buffer( (unsigned char*)malloc(mMp3BufferSize) )
{
#if 0
   mpQuality->setSingleStep( 0.01 );
   mpQuality->setMinimum( 0.0 );
   mpQuality->setMaximum( 1.0 );
   mpQuality->setToolTip( tr("0 = low & short, 1 = high & long") );
#endif

   QGridLayout *mainLayout = new QGridLayout( mpConfigWidget );
#if QT_VERSION < 0x040300
   mainLayout->setMargin( 3 );
#else
   mainLayout->setContentsMargins( 3, 3, 3, 3 );
#endif
   mainLayout->addWidget( mpUseEncoder, 0, 0, 1, 3 );
   mainLayout->addWidget( mpDirOverride, 1, 0, 1, 3 );
   mainLayout->addWidget( new QLabel( tr("Base Directory:") ), 2, 0 );
   mainLayout->addWidget( mpDirectory, 2, 1 );
   mainLayout->addWidget( mpDotButton, 2, 2 );
   mainLayout->addWidget( mpUseLatin1, 3, 0, 1, 3 );
   mainLayout->setColumnStretch( 1, 1 );
   mainLayout->setRowStretch( 4, 1 );

   mpConfigWidget->setLayout( mainLayout );

   connect( mpUseEncoder, SIGNAL(clicked(bool)),
            this, SIGNAL(useEncoderClicked(bool)) );
   connect( mpDotButton, SIGNAL(clicked()),
            this, SLOT(handleDotButton()) );

   readSettings();
}


Mp3Encoder::~Mp3Encoder()
{
   free( mpMp3Buffer );
   mpMp3Buffer = 0;
}


void Mp3Encoder::setUseEncoder( bool on )
{
   mpUseEncoder->setChecked( on );
}


void Mp3Encoder::readSettings()
{
   MySettings settings;
   settings.beginGroup( mName );
   mUseEncoder  = settings.VALUE_USE_ENCODER;
   mDirOverride = settings.VALUE_DIRECTORY_OVERRIDE;
   mDirectory   = settings.VALUE_DIRECTORY;
   mUseLatin1   = settings.VALUE_USE_LATIN1;
   mpUseEncoder->setChecked( mUseEncoder );
   mpDirOverride->setChecked( mDirOverride );
   mpDirectory->setText( mDirectory );
   mpUseLatin1->setChecked( mUseLatin1 );
   settings.endGroup();
}


void Mp3Encoder::writeSettings()
{
   MySettings settings;
   settings.beginGroup( mName );
   mUseEncoder  = mpUseEncoder->isChecked();
   mDirOverride = mpDirOverride->isChecked();
   mDirectory   = mpDirectory->text();
   mUseLatin1   = mpUseLatin1->isChecked();
   settings.setValue( "UseEncoder", mUseEncoder );
   settings.setValue( "DirectoryOverride", mDirOverride );
   settings.setValue( "Directory", mDirectory );
   settings.setValue( "UseLatin1", mUseLatin1 );
   settings.endGroup();
}


bool Mp3Encoder::initialize( const QString &fileName )
{
   if( !Encoder::initialize( fileName, ".mp3" ) )
   {
      return false;
   }

   mLame = ::lame_init();
   if( !mLame )
   {
      return false;
   }
   ::lame_set_in_samplerate( mLame, 44100 );
//   ::lame_set_brate( mLame, 128 );
//   ::lame_set_compression_ratio( mLame, 11.0 );
   ::lame_set_VBR( mLame, mUseAbr ? vbr_abr : vbr_default );
//   ::lame_set_VBR_quality( mLame, 9.9 );  /* 0..9 0:high 9:low */
   ::lame_init_params( mLame );

   id3tag_init( mLame );
   for( int i = 0; i < mTagList.count(); i++ )
   {
      if( !mTagList.valueAt(i).isEmpty() )
      {
         if( mTagList.tagAt(i) == "ALBUM" )
         {
            if( mUseLatin1 )
            {
               ::id3tag_set_album(mLame, mTagList.valueAt(i).toLatin1() );
            }
            else
            {
               ::id3tag_set_album(mLame, mTagList.valueAt(i).toUtf8() );
            }
         }
         else if( mTagList.tagAt(i) == "ARTIST" )
         {
            if( mUseLatin1 )
            {
               ::id3tag_set_artist(mLame, mTagList.valueAt(i).toUtf8() );
            }
            else
            {
               ::id3tag_set_artist(mLame, mTagList.valueAt(i).toLatin1() );
            }
         }
         else if( mTagList.tagAt(i) == "TITLE" )
         {
            if( mUseLatin1 )
            {
               ::id3tag_set_title(mLame, mTagList.valueAt(i).toUtf8() );
            }
            else
            {
               ::id3tag_set_title(mLame, mTagList.valueAt(i).toLatin1() );
            }
         }
         else if( mTagList.tagAt(i) == "TRACKNUMBER" )
         {
            if( mUseLatin1 )
            {
               ::id3tag_set_track(mLame, mTagList.valueAt(i).toUtf8() );
            }
            else
            {
               ::id3tag_set_track(mLame, mTagList.valueAt(i).toLatin1() );
            }
         }
         else if( mTagList.tagAt(i) == "GENRE" )
         {
            if( mUseLatin1 )
            {
               ::id3tag_set_genre(mLame, mTagList.valueAt(i).toUtf8() );
            }
            else
            {
               ::id3tag_set_genre(mLame, mTagList.valueAt(i).toLatin1() );
            }
         }
         else if( mTagList.tagAt(i) == "DATE" )
         {
            if( mUseLatin1 )
            {
               ::id3tag_set_year(mLame, mTagList.valueAt(i).toUtf8() );
            }
            else
            {
               ::id3tag_set_year(mLame, mTagList.valueAt(i).toLatin1() );
            }
         }
      }
   }

   return true;
}


bool Mp3Encoder::finalize( bool enqueue, bool cancel )
{
   /* make sure all data is processed */
   wait();

   if( !encodeCDAudio( 0, 0 ) )
   {
      Encoder::finalize( false, true );
      return false;
   }

   qint64 write = ::lame_encode_flush(mLame, mpMp3Buffer, 0);
   if( !Encoder::writeChunk( (const char*)mpMp3Buffer, write ) )
   {
      Encoder::finalize( false, true );
      return false;
   }
   lame_close( mLame );
   return Encoder::finalize( enqueue, cancel );
}


void Mp3Encoder::encodeCDAudio( const QByteArray &data )
{
   if( !encodeCDAudio( data.constData(), data.size() ) )
   {
      emit encodingFail();
   }
}


bool Mp3Encoder::encodeCDAudio( const char* data, int size )
{
   short int *pcm = (short int*)data;
   int write = ::lame_encode_buffer_interleaved(mLame, pcm, size/4, mpMp3Buffer, mMp3BufferSize);
   if( !Encoder::writeChunk( (const char*)mpMp3Buffer, (qint64)write ) )
   {
      Encoder::finalize( false, true );
      return false;
   }
   return true;
}


QWidget *Mp3Encoder::configWidget()
{
   return mpConfigWidget;
}


void Mp3Encoder::handleDotButton()
{
   Encoder::setDirectory( mpDirectory );
}
