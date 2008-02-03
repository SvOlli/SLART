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
   void initialize( const QString &fileName );
   void finalize();
   void setTag( const QString &tag, const QString &value );
   void encodeCDAudio( const char* data, int size );
   const QString name;

public slots:
   void setQuality( double quality );
   
private:
   OggEncoder( const OggEncoder &other );
   OggEncoder &operator=( const OggEncoder &other );
   void oggInit();
   QString mEncoderName;

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
