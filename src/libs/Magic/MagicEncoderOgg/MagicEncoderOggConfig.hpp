/*
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


/*!
  \addtogroup MagicEncoderOgg

  @{
*/

/*!
 \brief configuration widget

*/
class MagicEncoderOggConfig : public MagicEncoderConfig
{
   Q_OBJECT

public:
   /*!
    \brief constructor

    \param encoder
    \param parent
    \param button
   */
   MagicEncoderOggConfig( MagicEncoderOgg *encoder, QWidget *parent, QAbstractButton *button );

   /*!
    \brief destructor

   */
   virtual ~MagicEncoderOggConfig();

   /*!
    \brief read settings from storage

   */
   void readSettings();

   /*!
    \brief write settings to storage

   */
   void writeSettings();

private:
   Q_DISABLE_COPY( MagicEncoderOggConfig )

   MagicEncoderOgg      *mpEncoder; /*!< \brief handle for encoder */
   QDoubleSpinBox       *mpQuality; /*!< \brief widget for quality setting */
};

/*! @} */

#define VALUE_OGGQUALITY         value( "OggQuality", 0.4 ).toDouble()

#endif
