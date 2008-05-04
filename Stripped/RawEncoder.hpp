/**
 * RawEncoder.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#ifndef RAWRawEncoder_HPP
#define RAWRawEncoder_HPP RAWRawEncoder_HPP

#include "Encoder.hpp"
#include <QWidget>

class RawEncoder : public Encoder
{
Q_OBJECT
   
public:
   RawEncoder( QWidget *parent = 0 );
   virtual ~RawEncoder();

   /* initialize the encoder */
   void initialize( const QString &fileName );
   /* finalize (clean up) the encoder */
   void finalize( bool enqueue );
   /* set the tags of the encoded file */
   void setTags( const TagList &tagList );
   /* encode raw cd audio data */
   bool encodeCDAudio( const char* data, int size );

private:
   RawEncoder( const RawEncoder &other );
   RawEncoder &operator=( const RawEncoder &other );
};

#endif
