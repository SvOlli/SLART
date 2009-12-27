/**
 * src/apps/Sorcerer/Plugin.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#include "MainWindow.hpp"
#include "MainWidget.hpp"
#include "MySettings.hpp"

#include <QtGui>

#include "Plugin.hpp"

int SorcererPlugin::run( QApplication *app )
{
   MainWindow window( false );
   MainWidget *mainWidget = new MainWidget( &window );
   window.setMainWidget( mainWidget );
   window.changeTitle( QIcon(),
                       QString("Sorcerer (running for %1)").arg(app->applicationName()) );
   window.show();
   app->exec();
   
   return mainWidget->errors();
}

Q_EXPORT_PLUGIN2(Sorcerer, SorcererPlugin)
