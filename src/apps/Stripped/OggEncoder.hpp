/**
 * src/apps/Stripped/OggEncoder.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef RAWOggEncoder_HPP
#define RAWOggEncoder_HPP RAWOggEncoder_HPP

extern "C"
{
#include <vorbis/vorbisenc.h>
}

#include "Encoder.hpp"


class OggEncoder : public Encoder
{
Q_OBJECT
   
public:
   OggEncoder( QWidget *parent = 0 );
   virtual ~OggEncoder();
   
   /* initialize the encoder */
   bool initialize( const QString &fileName );
   /* finalize (clean up) the encoder */
   bool finalize( bool enqueue, bool cancel );
   /* set the tags of the encoded file */
   bool setTags( const TagList &tagList );
   /* encode raw cd audio data */
   bool encodeCDAudio( const char* data, int size );
   
public slots:
   /* set the encoding quality */
   void setQuality( double quality );
   
private:
   OggEncoder( const OggEncoder &other );
   OggEncoder &operator=( const OggEncoder &other );
   
   /* ogg initialize helper function call on first encode */
   bool oggInit();
   
   ::ogg_stream_state   mOggPagegStream;
   ::ogg_page           mOggPage;
   ::ogg_packet         mOggPacket;
   ::vorbis_info        mVorbisInfo;
   ::vorbis_comment     mVorbisComment;
   ::vorbis_dsp_state   mVorbisDspState;
   ::vorbis_block       mVorbisBlock;
   
   bool   mIsInit;
   float  mQuality;
};

#endif
