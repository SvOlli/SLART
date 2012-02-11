/*
 * src/libs/Sorcerer/SorcererInterface.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef SORCERERINTERFACE_HPP
#define SORCERERINTERFACE_HPP SORCERERINTERFACE_HPP

/* system headers */

/* Qt headers */
class QApplication;

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */

/* forward declaration of local classes */

/*!
  \addtogroup Sorcerer Sorcerer: setup wizard
  @{
  */


/*!
 \brief plugin interface for Sorcerer, the setup wizard

*/
class SorcererInterface
{
public:
   /*!
    \brief destructor

   */
   virtual ~SorcererInterface() {}
   /*!
    \brief run the setup wizard

   */
   virtual int setup() = 0;
   /*!
    \brief cleanup of obsolete registry settings

   */
   virtual void cleanup() = 0;
   /*!
    \brief installation of hidden registry settings

   */
   virtual void hidden() = 0;
};

Q_DECLARE_INTERFACE(SorcererInterface,
                    "org.svolli.SLART.SorcererInterface/1.0")

/*! @} */

#endif
