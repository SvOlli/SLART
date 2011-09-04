/**
 * src/apps/Magic/MagicEncoderMp3/MagicEncoderMp3Config.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef MAGICENCODERMP3CONFIG_HPP
#define MAGICENCODERMP3CONFIG_HPP MAGICENCODERMP3CONFIG_HPP

/* base class */
#include <MagicEncoderConfig.hpp>

/* system headers */

/* Qt headers */

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */
class QCheckBox;
class QDoubleSpinBox;

/* forward declaration of local classes */
class MagicEncoderMp3;


class MagicEncoderMp3Config : public MagicEncoderConfig
{
   Q_OBJECT

public:
   MagicEncoderMp3Config( MagicEncoderMp3 *encoder, QWidget *parent, QAbstractButton *button );
   virtual ~MagicEncoderMp3Config();

   /* read settings from storage */
   void readSettings();
   /* write settings to storage */
   void writeSettings();

private:
   MagicEncoderMp3Config( const MagicEncoderMp3Config &that );
   MagicEncoderMp3Config &operator=( const MagicEncoderMp3Config &that );

   MagicEncoderMp3      *mpEncoder;

   /* configuration widget */
   QDoubleSpinBox       *mpQuality;
   QCheckBox            *mpUseLatin1;
};

#define VALUE_USE_LATIN1         value( "UseLatin1", false ).toBool()
#define VALUE_VBRQUALITY         value( "VBRQuality", 4.0 ).toDouble()

#endif
