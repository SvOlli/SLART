/**
 * OggEncoder.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#ifndef RAWOggEncoder_HPP
#define RAWOggEncoder_HPP RAWOggEncoder_HPP

#include "Encoder.hpp"

extern "C"
{
#include <vorbis/vorbisenc.h>
};

class OggEncoder : public Encoder
{
Q_OBJECT

public:
   OggEncoder( QWidget *parent = 0 );
   virtual ~OggEncoder();

   /* initialize the encoder */
   void initialize( const QString &fileName );
   /* finalize (clean up) the encoder */
   void finalize( bool enqueue );
   /* set the tags of the encoded file */
   void setTags( const TagList &tagList );
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

   ::ogg_stream_state   mOS;
   ::ogg_page           mOG;
   ::ogg_packet         mOP;
   ::vorbis_info        mVI;
   ::vorbis_comment     mVC;
   ::vorbis_dsp_state   mVD;
   ::vorbis_block       mVB;
   
   bool   mIsInit;
   float  mQuality;
};

#endif
