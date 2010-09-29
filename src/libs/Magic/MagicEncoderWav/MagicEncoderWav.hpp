/**
 * src/apps/Magic/MagicEncoderWav/MagicEncoderWav.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
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
   MagicEncoderWav( const MagicEncoderWav &other );
   MagicEncoderWav &operator=( const MagicEncoderWav &other );

   /* configuration widget */
   MagicEncoderConfig   *mpConfigWidget;

   /* encoder internal data */
   unsigned int         *mpWavHeader;
};

#endif
