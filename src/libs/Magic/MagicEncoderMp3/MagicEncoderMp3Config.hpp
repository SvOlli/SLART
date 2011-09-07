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


/*!
  \addtogroup MagicEncoderMp3

  @{
*/

/*!
 \brief configuration widget

*/
class MagicEncoderMp3Config : public MagicEncoderConfig
{
   Q_OBJECT

public:
   /*!
    \brief constructor

    \param encoder
    \param parent
    \param button
   */
   MagicEncoderMp3Config( MagicEncoderMp3 *encoder, QWidget *parent, QAbstractButton *button );

   /*!
    \brief destructor

   */
   virtual ~MagicEncoderMp3Config();

   /*!
    \brief read settings from storage

   */
   void readSettings();
   /*!
    \brief write settings to storage

   */
   void writeSettings();

private:
   Q_DISABLE_COPY( MagicEncoderMp3Config )

   MagicEncoderMp3      *mpEncoder; /*!< \brief handle for encoder */
   QDoubleSpinBox       *mpQuality; /*!< \brief quality control widget */
   QCheckBox            *mpUseLatin1; /*!< \brief use latin1 widget */
};

/*! @} */

#define VALUE_USE_LATIN1         value( "UseLatin1", false ).toBool()
#define VALUE_VBRQUALITY         value( "VBRQuality", 4.0 ).toDouble()

#endif
