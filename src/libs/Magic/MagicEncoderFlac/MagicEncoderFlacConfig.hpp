/**
 * src/apps/Magic/MagicEncoderFlac/MagicEncoderFlacConfig.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef MAGICENCODERFLACCONFIG_HPP
#define MAGICENCODERFLACCONFIG_HPP MAGICENCODERFLACCONFIG_HPP

/* base class */
#include <MagicEncoderConfig.hpp>

/* system headers */

/* Qt headers */

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */
class QCheckBox;
class QSpinBox;

/* forward declaration of local classes */
class MagicEncoderFlac;
class ScrollLine;


class MagicEncoderFlacConfig : public MagicEncoderConfig
{
Q_OBJECT

public:
   MagicEncoderFlacConfig( MagicEncoderFlac *encoder, QWidget *parent, QAbstractButton *button );
   virtual ~MagicEncoderFlacConfig();

   /* read settings from storage */
   void readSettings();
   /* write settings to storage */
   void writeSettings();

private:
   MagicEncoderFlacConfig( const MagicEncoderFlacConfig &other );
   MagicEncoderFlacConfig &operator=( const MagicEncoderFlacConfig &other );

   MagicEncoderFlac     *mpEncoder;

   /* configuration widget */
   QSpinBox             *mpQuality;
   QCheckBox            *mpUseOga;
};

#define VALUE_FLACQUALITY        value( "FlacQuality", 5 ).toInt()
#define VALUE_FLACUSEOGA         value( "FlacUseOga", false ).toBool()

#endif
