/*
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


/*!
  \addtogroup Magic

  @{
*/

/*!
 \brief configuration widget

*/
class MagicEncoderFlacConfig : public MagicEncoderConfig
{
   Q_OBJECT

public:
   /*!
    \brief constructor

    \param encoder
    \param parent
    \param button
   */
   MagicEncoderFlacConfig( MagicEncoderFlac *encoder, QWidget *parent, QAction *toggleEnableAction );

   /*!
    \brief destructor

   */
   virtual ~MagicEncoderFlacConfig();

   /*!
    \brief read settings from storage

   */
   void readSettings();
   /*!
    \brief write settings to storage

   */
   void writeSettings();

private:
   Q_DISABLE_COPY( MagicEncoderFlacConfig )

   MagicEncoderFlac     *mpEncoder; /*!< \brief handle for encoder */
   QSpinBox             *mpQuality; /*!< \brief widget for quality setting */
   QCheckBox            *mpUseOga; /*!< \brief widget for changing container format */
};

/*! @} */

#endif
