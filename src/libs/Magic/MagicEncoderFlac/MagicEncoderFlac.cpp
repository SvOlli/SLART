/**
 * src/apps/Magic/MagicEncoderFlac/MagicEncoderFlac.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

/* class declaration */
#include "MagicEncoderFlac.hpp"

/* system headers */
#include <cstdio>

/* Qt headers */
#include <QtGui>

/* local library headers */
#include <MySettings.hpp>
#include <ScrollLine.hpp>
#include <TagList.hpp>

/* local headers */
#include "MagicEncoderFlacConfig.hpp"


MagicEncoderFlac::MagicEncoderFlac()
: MagicEncoder( tr("FLAC") )
, mQuality( 0 )
, mUseOga( false )
, mPcmSize( 0 )
, mpConfigWidget()
, mpEncoder( 0 )
, mpMetadata( 0 )
, mpPcm( 0 )
{
   qsrand( time((time_t*)0) );
}


MagicEncoderFlac::~MagicEncoderFlac()
{
   if( mpConfigWidget )
   {
      delete mpConfigWidget;
   }
   if( mpEncoder )
   {
      delete mpEncoder;
      mpEncoder = 0;
   }
   if( mpMetadata )
   {
      delete mpMetadata;
      mpMetadata = 0;
   }
   if( mpPcm )
   {
      delete[] mpPcm;
      mpPcm = 0;
   }
}


bool MagicEncoderFlac::initialize( const QString &fileName )
{
   bool                                     ok = true;
   FLAC__StreamMetadata                     *metadata[2];
   FLAC__StreamMetadata_VorbisComment_Entry entry;

   if( !mpEncoder )
   {
      mpEncoder = new FLAC::Encoder::File();
   }

   if( mUseOga )
   {
      if( !MagicEncoder::initialize( fileName, ".oga" ) )
      {
         return false;
      }
      ok &= mpEncoder->set_ogg_serial_number( qrand() );
   }
   else
   {
      if( !MagicEncoder::initialize( fileName, ".flac" ) )
      {
         return false;
      }
   }

   ok &= mpEncoder->set_verify(true);
   ok &= mpEncoder->set_compression_level(mQuality);
   ok &= mpEncoder->set_channels(2);
   ok &= mpEncoder->set_bits_per_sample(16);
   ok &= mpEncoder->set_sample_rate(44100);

   metadata[0] = FLAC__metadata_object_new(FLAC__METADATA_TYPE_VORBIS_COMMENT);
   for( int i = 0; i < mTagList.count(); i++ )
   {
      if( !mTagList.valueAt(i).isEmpty() )
      {
         ok &= FLAC__metadata_object_vorbiscomment_entry_from_name_value_pair(
               &entry,
               mTagList.tagAt(i).toUtf8().constData(),
               mTagList.valueAt(i).toUtf8().constData() );
         ok &= FLAC__metadata_object_vorbiscomment_append_comment(
               metadata[0], entry, /*copy=*/false);
               /* copy=false: let metadata object take control of entry's allocated string */
      }
   }

   ok &= mpEncoder->set_metadata(metadata, 1);

   if( mUseOga )
   {
      /* not working yet, need to obtain ogg_serial */
      mpEncoder->set_ogg_serial_number( 0 );
      ok &= mpEncoder->init_ogg( ::fdopen( mFile.handle(), "w" ) );
   }
   else
   {
      ok &= mpEncoder->init( ::fdopen( mFile.handle(), "w" ) );
   }

   return ok;
}


bool MagicEncoderFlac::finalize( bool enqueue, bool cancel )
{
   /* make sure all data is processed */
   wait();

   bool ok( true );
   ok &= mpEncoder->finish();
   ok &= MagicEncoder::finalize( enqueue, cancel );
   return ok;
}


void MagicEncoderFlac::encodeCDAudio( const QByteArray &data )
{
   if( !encodeCDAudio( data.constData(), data.size() ) )
   {
      emit encodingFail();
   }
}


bool MagicEncoderFlac::encodeCDAudio( const char* data, int size )
{
   bool ok( true );
   FLAC__byte  *buffer = (FLAC__byte*)data;
   if( size > mPcmSize )
   {
      if( mpPcm )
      {
         delete[] mpPcm;
         mpPcm = 0;
      }
      mpPcm  = new FLAC__int32[size/2];
      mPcmSize = size;
   }

   for( int i = 0; i < size/2; i++ )
   {
      /* inefficient but simple and works on big- or little-endian machines */
      mpPcm[i] = (FLAC__int32)(((FLAC__int16)(FLAC__int8)buffer[2*i+1] << 8) |
                 (FLAC__int16)buffer[2*i]);
   }
   /* feed samples to encoder */
   ok &= mpEncoder->process_interleaved( mpPcm, size/4 );

   return ok;
}


MagicEncoderConfig *MagicEncoderFlac::configWidget( QWidget *parent, QAbstractButton *button )
{
   if( !mpConfigWidget )
   {
      mpConfigWidget = new MagicEncoderFlacConfig( this, parent, button );
   }
   return mpConfigWidget;
}


Q_EXPORT_PLUGIN2(MagicEncoder, MagicEncoderFlac)
