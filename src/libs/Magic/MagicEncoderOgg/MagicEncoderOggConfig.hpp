/**
 * src/apps/Magic/MagicEncoderOgg/MagicEncoderOgg.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
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
   MagicEncoderOggConfig( const MagicEncoderOggConfig &other );
   MagicEncoderOggConfig &operator=( const MagicEncoderOggConfig &other );
   
   MagicEncoderOgg      *mpEncoder;

   /* configuration widget */
   QDoubleSpinBox       *mpQuality;
};

#define VALUE_OGGQUALITY         value( "OggQuality", 0.4 ).toDouble()

#endif
