/*
 * src/apps/Magic/MagicEncoderMp3/MagicEncoderMp3.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef MAGICENCODERMP3_HPP
#define MAGICENCODERMP3_HPP MAGICENCODERMP3_HPP

/* base class */
#include <MagicEncoder.hpp>

/* system headers */
extern "C" {
#include <lame/lame.h>
}

/* Qt headers */
#include <QPointer>

/* local library headers */

/* local headers */
#include "MagicEncoderMp3Config.hpp"

/* forward declaration of Qt classes */

/* forward declaration of local classes */

/*!
  \addtogroup Magic
  @{
  */


/*!
 \brief plugin for encoding raw audio data to mp3

*/
class MagicEncoderMp3 : public MagicEncoder
{
   Q_OBJECT
   Q_INTERFACES(MagicEncoderInterface)
   Q_PLUGIN_METADATA(IID "org.svolli.slart.magic.encoder.mp3")

   friend class MagicEncoderMp3Config;

public:
   /*!
    \brief constructor

    */
   MagicEncoderMp3();

   /*!
    \brief destructor

   */
   virtual ~MagicEncoderMp3();

   /*!
    \brief supply the a handle to the configuration widget

    \param parent
    \param button
   */
   MagicEncoderConfig *configWidget( QWidget *parent );

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
   Q_DISABLE_COPY( MagicEncoderMp3 )

   /*!
    \brief initialize

   */
   bool initialize();

   /*!
    \brief encode raw cd audio data

    \param data
    \param size
   */
   bool encodeCDAudio( const char *data, int size );

   /*!
    \brief convert tag to latin1 or utf8

    \param key key of tag to convert
   */
   QByteArray tagTo8Bit( const QByteArray &key );

   bool                          mUseAbr; /*!< \brief use average bit rate */
   bool                          mUseLatin1; /*!< \brief use latin1 */
   float                         mQuality; /*!< \brief quality */
   QPointer<MagicEncoderConfig>  mpConfigWidget; /*!< \brief configuration widget */
   volatile lame_t               mLame; /*!< \brief lame handle */
   const int                     mMp3BufferSize; /*!< \brief mp3 buffer size */
   unsigned char                 *mpMp3Buffer; /*!< \brief mp3 buffer */
};

/*! @} */

#endif
