/**
 * src/libs/Sorcerer/SorcererInterface.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
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


class SorcererInterface
{
public:
   virtual ~SorcererInterface() {}
   virtual int setup( QApplication *app ) = 0;
   virtual void cleanup( QApplication *app ) = 0;
   virtual void hidden() = 0;
};

Q_DECLARE_INTERFACE(SorcererInterface,
                    "org.svolli.SLART.SorcererInterface/1.0")

#endif
