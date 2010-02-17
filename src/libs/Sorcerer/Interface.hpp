/**
 * src/libs/Sorcerer/Interface.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef SORCERERINTERFACE_HPP
#define SORCERERINTERFACE_HPP SORCERERINTERFACE_HPP

class QApplication;


class SorcererInterface
{
public:
   virtual ~SorcererInterface() {}
   virtual int run( QApplication *app ) = 0;
};

Q_DECLARE_INTERFACE(SorcererInterface,
                    "org.svolli.SLART.SorcererInterface/1.0")

#endif
