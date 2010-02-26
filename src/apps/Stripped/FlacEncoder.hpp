/**
 * src/apps/Stripped/FlacEncoder.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef FLACENCODER_HPP
#define FLACENCODER_HPP FLACENCODER_HPP

#include "Encoder.hpp"

#include "FLAC++/metadata.h"
#include "FLAC++/encoder.h"

#include <QWidget>


class FlacEncoder : public Encoder
{
Q_OBJECT

public:
   FlacEncoder( QWidget *parent = 0 );
   virtual ~FlacEncoder();

   /* initialize the encoder */
   bool initialize( const QString &fileName );
   /* finalize (clean up) the encoder */
   bool finalize( bool enqueue, bool cancel );
   /* encode raw cd audio data */
   bool encodeCDAudio( const char* data, int size );

public slots:
   /* set the encoding quality */
   void setQuality( int quality );

private:
   FlacEncoder( const FlacEncoder &other );
   FlacEncoder &operator=( const FlacEncoder &other );

   bool                 mUseOggContainer;
   FLAC::Encoder::File  *mpEncoder;
   FLAC__StreamMetadata *mpMetadata;
   FLAC__int32          *mpPcm;
   int                  mSize;
   int                  mQuality;
};

#endif
