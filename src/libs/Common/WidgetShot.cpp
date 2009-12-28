/**
 * src/libs/Common/WidgetShot.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#include "WidgetShot.hpp"

#include <QApplication>
#include <QFile>
#include <QPixmap>


QHash<QString,QWidget*> *WidgetShot::mWidgetList = 0;


void WidgetShot::addWidget( const QString &widgetName, QWidget *widget )
{
   if( !mWidgetList )
   {
      mWidgetList = new QHash<QString,QWidget*>();
   }
   mWidgetList->insert( QApplication::applicationName() + ":" + widgetName, widget );
}


void WidgetShot::shootWidget( const QString &widgetName, const QString &fileName )
{
   if( !mWidgetList )
   {
      return;
   }
   QWidget *widget = mWidgetList->value( widgetName, 0 );
   if( !widget )
   {
      return;
   }
   QPixmap pixmap( QPixmap::grabWidget( widget ) );
   QFile file( fileName );
   if( file.exists() )
   {
      return;
   }
   pixmap.save( fileName );
}
