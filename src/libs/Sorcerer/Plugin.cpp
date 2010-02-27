/**
 * src/libs/Sorcerer/Plugin.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "Plugin.hpp"

/* system headers */

/* Qt headers */
#include <QtGui>

/* local library headers */
#include <MainWindow.hpp>
#include <MySettings.hpp>

/* local headers */
#include "SorcererWidget.hpp"


int SorcererPlugin::run( QApplication *app )
{
   MainWindow window( false );
   SorcererWidget *sorcererWidget = new SorcererWidget( &window );
   window.setMainWidget( sorcererWidget );
   if( app->applicationName() == "Sorcerer" )
   {
      window.changeTitle( QIcon(), app->applicationName() );
   }
   else
   {
      window.changeTitle( QIcon(),
                          QString("Sorcerer (running for %1)").arg(app->applicationName()) );
   }
   window.show();
   app->exec();
   
   return sorcererWidget->errors();
}

Q_EXPORT_PLUGIN2(Sorcerer, SorcererPlugin)
