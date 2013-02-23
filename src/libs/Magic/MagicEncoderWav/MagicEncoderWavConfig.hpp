/*
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


/*!
  \addtogroup Magic

  @{
*/

/*!
 \brief configuration widget

*/
class MagicEncoderWavConfig : public MagicEncoderConfig
{
   Q_OBJECT

public:
   /*!
    \brief constructor

    \param encoder
    \param parent
    \param button
   */
   MagicEncoderWavConfig( MagicEncoderWav *encoder, QWidget *parent, QAction *toggleEnableAction );

   /*!
    \brief destructor

   */
   virtual ~MagicEncoderWavConfig();

   /*!
    \brief read settings from storage

   */
   void readSettings();

   /*!
    \brief write settings to storage

   */
   void writeSettings();

private:
   Q_DISABLE_COPY( MagicEncoderWavConfig )

   MagicEncoderWav      *mpEncoder; /*!< \brief handle for encoder */

};

/*! @} */

#endif
