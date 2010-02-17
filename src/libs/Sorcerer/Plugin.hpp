/**
 * src/apps/Sorcerer/Plugin.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef PLUGIN_HPP
#define PLUGIN_HPP PLUGIN_HPP

#include <QWidget>
#include "Interface.hpp"

#include <QObject>


class SorcererPlugin : public QObject, public SorcererInterface
{
   Q_OBJECT
   Q_INTERFACES(SorcererInterface)
   
public:
   int run( QApplication *app );
};

#endif
