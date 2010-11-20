/**
 * src/apps/Magic/MagicEncoderOgg/MagicEncoderOgg.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef MAGICENCODEROGG_HPP
#define MAGICENCODEROGG_HPP MAGICENCODEROGG_HPP

/* base class */
#include <MagicEncoder.hpp>

/* system headers */
extern "C" {
#include <vorbis/vorbisenc.h>
}

/* Qt headers */
#include <QPointer>

/* local library headers */

/* local headers */
#include "MagicEncoderOggConfig.hpp"

/* forward declaration of Qt classes */

/* forward declaration of local classes */


class MagicEncoderOgg : public MagicEncoder
{
Q_OBJECT
Q_INTERFACES(MagicEncoderInterface)

   friend class MagicEncoderOggConfig;

public:
   MagicEncoderOgg();
   virtual ~MagicEncoderOgg();

   /* supply the a handle to the configuration widget */
   MagicEncoderConfig *configWidget( QWidget *parent, QAbstractButton *button );
   /* initialize the encoder */
   bool initialize( const QString &fileName );
   /* finalize (clean up) the encoder */
   bool finalize( bool enqueue, bool cancel );

   /* encode raw cd audio data */
   void encodeCDAudio( const QByteArray &data );

private:
   MagicEncoderOgg( const MagicEncoderOgg &that );
   MagicEncoderOgg &operator=( const MagicEncoderOgg &that );

   /* ogg initialize helper function call on first encode */
   bool oggInit();
   /* encode raw cd audio data */
   bool encodeCDAudio( const char *data, int size );

   /* settings */
   float                         mQuality;

   /* configuration widget */
   QPointer<MagicEncoderConfig>  mpConfigWidget;

   /* encoder internal data */
   bool                          mIsInit;
   ::ogg_stream_state            mOggPagegStream;
   ::ogg_page                    mOggPage;
   ::ogg_packet                  mOggPacket;
   ::vorbis_info                 mVorbisInfo;
   ::vorbis_comment              mVorbisComment;
   ::vorbis_dsp_state            mVorbisDspState;
   ::vorbis_block                mVorbisBlock;
};

#endif
