/**
 * src/apps/Magic/MagicEncoderOgg/MagicEncoderOgg.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef MAGICENCODEROGGCONFIG_HPP
#define MAGICENCODEROGGCONFIG_HPP MAGICENCODEROGGCONFIG_HPP

/* base class */
#include <MagicEncoderConfig.hpp>

/* system headers */

/* Qt headers */

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */
class QDoubleSpinBox;

/* forward declaration of local classes */
class MagicEncoderOgg;


class MagicEncoderOggConfig : public MagicEncoderConfig
{
Q_OBJECT

public:
   MagicEncoderOggConfig( MagicEncoderOgg *encoder, QWidget *parent, QAbstractButton *button );
   virtual ~MagicEncoderOggConfig();

   /* read settings from storage */
   void readSettings();
   /* write settings to storage */
   void writeSettings();

private:
   MagicEncoderOggConfig( const MagicEncoderOggConfig &that );
   MagicEncoderOggConfig &operator=( const MagicEncoderOggConfig &that );
   
   MagicEncoderOgg      *mpEncoder;

   /* configuration widget */
   QDoubleSpinBox       *mpQuality;
};

#define VALUE_OGGQUALITY         value( "OggQuality", 0.4 ).toDouble()

#endif
