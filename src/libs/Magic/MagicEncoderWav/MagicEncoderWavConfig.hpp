/**
 * src/apps/Magic/MagicEncoderWav/MagicEncoderWavConfig.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef MAGICENCODERWAVCONFIG_HPP
#define MAGICENCODERWAVCONFIG_HPP MAGICENCODERWAVCONFIG_HPP

/* base class */
#include <MagicEncoderConfig.hpp>

/* system headers */

/* Qt headers */

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */
class QCheckBox;

/* forward declaration of local classes */
class MagicEncoderWav;
class ScrollLine;


class MagicEncoderWavConfig : public MagicEncoderConfig
{
Q_OBJECT

public:
   MagicEncoderWavConfig( MagicEncoderWav *encoder, QWidget *parent, QAbstractButton *button );
   virtual ~MagicEncoderWavConfig();

   /* read settings from storage */
   void readSettings();
   /* write settings to storage */
   void writeSettings();

private:
   MagicEncoderWavConfig( const MagicEncoderWavConfig &other );
   MagicEncoderWavConfig &operator=( const MagicEncoderWavConfig &other );

   MagicEncoderWav      *mpEncoder;

};

#endif
