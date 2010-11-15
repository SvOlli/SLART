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


class MagicEncoderFlac : public MagicEncoder
{
Q_OBJECT
Q_INTERFACES(MagicEncoderInterface)

   friend class MagicEncoderFlacConfig;

public:
   MagicEncoderFlac();
   virtual ~MagicEncoderFlac();

   /* supply the a handle to the configuration widget */
   MagicEncoderConfig *configWidget( QWidget *parent, QAbstractButton *button );
   /* initialize the encoder */
   bool initialize( const QString &fileName );
   /* finalize (clean up) the encoder */
   bool finalize( bool enqueue, bool cancel );

   /* encode raw cd audio data */
   void encodeCDAudio( const QByteArray &data );

private:
   MagicEncoderFlac( const MagicEncoderFlac &that );
   MagicEncoderFlac &operator=( const MagicEncoderFlac &that );

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

#endif
