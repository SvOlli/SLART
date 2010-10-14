/**
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


class MagicEncoderMp3 : public MagicEncoder
{
Q_OBJECT
Q_INTERFACES(MagicEncoderInterface)

   friend class MagicEncoderMp3Config;

public:
   MagicEncoderMp3();
   virtual ~MagicEncoderMp3();

   /* supply the a handle to the configuration widget */
   MagicEncoderConfig *configWidget( QWidget *parent, QAbstractButton *button );
   /* initialize the encoder */
   bool initialize( const QString &fileName );
   /* finalize (clean up) the encoder */
   bool finalize( bool enqueue, bool cancel );

   /* encode raw cd audio data */
   void encodeCDAudio( const QByteArray &data );

private:
   MagicEncoderMp3( const MagicEncoderMp3 &other );
   MagicEncoderMp3 &operator=( const MagicEncoderMp3 &other );

   bool initialize();
   /* encode raw cd audio data */
   bool encodeCDAudio( const char *data, int size );
   /* convert tag #i to latin1 or utf8 */
   QByteArray tagTo8Bit( int i );

   /* settings */
   bool                 mUseAbr;
   bool                 mUseLatin1;
   float                mQuality;

   /* configuration widget */
   QPointer<MagicEncoderConfig>  mpConfigWidget;

   /* encoder internal data */
   volatile lame_t      mLame;
   const int            mMp3BufferSize;
   unsigned char        *mpMp3Buffer;
};

#endif
