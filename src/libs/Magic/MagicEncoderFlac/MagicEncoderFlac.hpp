/**
 * src/apps/Magic/MagicEncoderFlac/MagicEncoderFlac.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef MAGICENCODERFLAC_HPP
#define MAGICENCODERFLAC_HPP MAGICENCODERFLAC_HPP

/* base class */
#include <MagicEncoder.hpp>

/* system headers */
#include "FLAC++/metadata.h"
#include "FLAC++/encoder.h"

/* Qt headers */
#include <QPointer>

/* local library headers */

/* local headers */
#include "MagicEncoderFlacConfig.hpp"

/* forward declaration of Qt classes */

/* forward declaration of local classes */

/*!
  \addtogroup MagicEncoderFlac MagicEncoderFlac: interface for FLAC audio encoding
  @{
  */


class MagicEncoderFlac : public MagicEncoder
{
   Q_OBJECT
   Q_INTERFACES(MagicEncoderInterface)

   friend class MagicEncoderFlacConfig;

public:
   MagicEncoderFlac();
   virtual ~MagicEncoderFlac();

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
   Q_DISABLE_COPY( MagicEncoderFlac )

   /* encode raw cd audio data */
   bool encodeCDAudio( const char *data, int size );

   /* settings */
   int                           mQuality;
   bool                          mUseOga;
   int                           mSize;

   /* configuration widget */
   QPointer<MagicEncoderConfig>  mpConfigWidget;

   /* encoder internal data */
   FLAC::Encoder::File           *mpEncoder;
   FLAC__StreamMetadata          *mpMetadata;
   FLAC__int32                   *mpPcm;
};

/*! @} */

#endif
