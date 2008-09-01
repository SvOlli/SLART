/**
 * Application.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#include "MainWindow.hpp"
#include "MainWidget.hpp"
#include "MySettings.hpp"

#include <QtGui>

#include "Plugin.hpp"

int SorcererPlugin::run( QApplication *app )
{
   MainWindow window( false );
   window.changeTitle( QIcon(),
                       QString("Sorcerer (running for %1)").arg(app->applicationName()) );
   window.show();
   app->exec();
   
   return window.mainWidget()->errors();
}

Q_EXPORT_PLUGIN2(Sorcerer, SorcererPlugin)
