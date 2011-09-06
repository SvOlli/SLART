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

/*!
  \addtogroup MagicEncoderOgg MagicEncoderOgg: interface for OggVorbis audio encoding
  @{
  */

/*!
 \brief

*/
class MagicEncoderOgg : public MagicEncoder
{
   Q_OBJECT
   Q_INTERFACES(MagicEncoderInterface)

   friend class MagicEncoderOggConfig;

public:
   /*!
    \brief constructor

   */
   MagicEncoderOgg();

   /*!
    \brief destructor

   */
   virtual ~MagicEncoderOgg();

   /*!
    \brief supply the a handle to the configuration widget

    \param parent
    \param button
   */
   MagicEncoderConfig *configWidget( QWidget *parent, QAbstractButton *button );

   /*!
    \brief initialize the encoder

    \param fileName
   */
   bool initialize( const QString &fileName );

   /*!
    \brief finalize (clean up) the encoder

    \param enqueue
    \param cancel
   */
   bool finalize( bool enqueue, bool cancel );

   /*!
    \brief encode raw cd audio data

    \param data
   */
   void encodeCDAudio( const QByteArray &data );

private:
   Q_DISABLE_COPY( MagicEncoderOgg )

   /*!
    \brief ogg initialize helper function call on first encode

   */
   bool oggInit();

   /*!
    \brief encode raw cd audio data

    \param data
    \param size
   */
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

/*! @} */

#endif
