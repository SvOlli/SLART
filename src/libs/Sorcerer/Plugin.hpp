/**
 * src/libs/Sorcerer/Plugin.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef PLUGIN_HPP
#define PLUGIN_HPP PLUGIN_HPP

/* base class */
#include <QObject>

/* system headers */

/* Qt headers */
#include <QWidget>

/* local library headers */

/* local headers */
#include "Interface.hpp"

/* forward declaration of Qt classes */

/* forward declaration of local classes */


class SorcererPlugin : public QObject, public SorcererInterface
{
   Q_OBJECT
   Q_INTERFACES(SorcererInterface)
   
public:
   int run( QApplication *app );
};

#endif
