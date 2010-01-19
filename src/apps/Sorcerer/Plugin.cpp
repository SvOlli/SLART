/**
 * src/apps/Sorcerer/Plugin.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#include "MainWindow.hpp"
#include "SorcererWidget.hpp"
#include "MySettings.hpp"

#include <QtGui>

#include "Plugin.hpp"

int SorcererPlugin::run( QApplication *app )
{
   MainWindow window( false );
   SorcererWidget *sorcererWidget = new SorcererWidget( &window );
   window.setMainWidget( sorcererWidget );
   window.changeTitle( QIcon(),
                       QString("Sorcerer (running for %1)").arg(app->applicationName()) );
   window.show();
   app->exec();
   
   return sorcererWidget->errors();
}

Q_EXPORT_PLUGIN2(Sorcerer, SorcererPlugin)
