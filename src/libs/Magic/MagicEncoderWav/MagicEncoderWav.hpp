/**
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

/* local library headers */

/* local headers */
#include "MagicEncoderWavConfig.hpp"

/* forward declaration of Qt classes */

/* forward declaration of local classes */

/*!
  \addtogroup MagicEncoderFlac MagicEncoderWav: interface for wave audio encoding
  @{
  */



class MagicEncoderWav : public MagicEncoder
{
   Q_OBJECT
Q_INTERFACES(MagicEncoderInterface)

   friend class MagicEncoderWavConfig;

public:
   MagicEncoderWav();
   virtual ~MagicEncoderWav();

   /* supply the a handle to the configuration widget */
   MagicEncoderConfig *configWidget( QWidget *parent, QAbstractButton *button );
   /* initialize the encoder */
   bool initialize( const QString &fileName );
   /* finalize (clean up) the encoder */
   bool finalize( bool enqueue, bool cancel );

   /* encode raw cd audio data */
   void encodeCDAudio( const QByteArray &data );

private:
   MagicEncoderWav( const MagicEncoderWav &that );
   MagicEncoderWav &operator=( const MagicEncoderWav &that );

   /* configuration widget */
   MagicEncoderConfig   *mpConfigWidget;

   /* encoder internal data */
   unsigned int         *mpWavHeader;
};

/*! @} */

#endif
