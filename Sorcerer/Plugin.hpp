/**
 * MainWidget.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 */

#ifndef PLUGIN_HPP
#define PLUGIN_HPP PLUGIN_HPP

#include <QWidget>
#include "Interface.hpp"

#include <QObject>


class SorcererPlugin : public QObject, SorcererInterface
{
   Q_OBJECT
   Q_INTERFACES(SorcererInterface)
   
public:
   int run( QApplication *app );
};

#endif
