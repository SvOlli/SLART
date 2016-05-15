/*
 * src/apps/Magic/MagicEncoderWav/MagicEncoderWav.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef MAGICENCODERWAV_HPP
#define MAGICENCODERWAV_HPP MAGICENCODERWAV_HPP

/* base class */
#include <MagicEncoder.hpp>

/* system headers */

/* Qt headers */
#include <QPointer>

/* local library headers */

/* local headers */
#include "MagicEncoderWavConfig.hpp"

/* forward declaration of Qt classes */

/* forward declaration of local classes */

/*!
  \addtogroup Magic
  @{
  */

/*!
 \brief plugin for encoding raw audio data to wave

*/
class MagicEncoderWav : public MagicEncoder
{
   Q_OBJECT
   Q_INTERFACES(MagicEncoderInterface)
   Q_PLUGIN_METADATA(IID "org.svolli.slart.magic.encoder.wav")

   friend class MagicEncoderWavConfig;

public:
   /*!
    \brief constructor

   */
   MagicEncoderWav();

   /*!
    \brief destructor

   */
   virtual ~MagicEncoderWav();

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
   Q_DISABLE_COPY( MagicEncoderWav )

   QPointer<MagicEncoderConfig>  mpConfigWidget;   /*!< \brief configuration widget */
   unsigned int                  *mpWavHeader;     /*!< \brief wave header data */
};

/*! @} */

#endif
