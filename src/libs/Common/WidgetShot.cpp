/*
 * src/libs/Common/WidgetShot.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

/* class declaration */
#include "WidgetShot.hpp"

/* system headers */

/* Qt headers */
#include <QApplication>
#include <QFile>
#include <QPixmap>

/* local library headers */

/* local headers */

/* class variable instantiation */
QHash< QString, QPointer<QWidget> > *WidgetShot::cpWidgetList = 0;


void WidgetShot::addWidget( const QString &widgetName, QWidget *widget )
{
   if( !cpWidgetList )
   {
      cpWidgetList = new QHash< QString, QPointer<QWidget> >();
   }
   cpWidgetList->insert( widgetName, widget );
}


void WidgetShot::shootWidget( const QString &widgetName, const QString &fileName )
{
   if( !cpWidgetList )
   {
      return;
   }
   QWidget *widget = cpWidgetList->value( widgetName, 0 );
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
