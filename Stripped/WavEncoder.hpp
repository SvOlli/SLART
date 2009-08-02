/**
 * WavEncoder.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#ifndef WAVENCODER_HPP
#define WAVENCODER_HPP WAVENCODER_HPP

#include "Encoder.hpp"

#include <QWidget>


class WavEncoder : public Encoder
{
Q_OBJECT
   
public:
   WavEncoder( QWidget *parent = 0 );
   virtual ~WavEncoder();
   
   /* initialize the encoder */
   void initialize( const QString &fileName );
   /* finalize (clean up) the encoder */
   void finalize( bool enqueue, bool cancel );
   /* set the tags of the encoded file */
   void setTags( const TagList &tagList );
   /* encode raw cd audio data */
   bool encodeCDAudio( const char* data, int size );
   
private:
   WavEncoder( const WavEncoder &other );
   WavEncoder &operator=( const WavEncoder &other );
};

#endif
