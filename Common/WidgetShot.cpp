/**
 * WidgetShot.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#include "WidgetShot.hpp"

#include <QPixmap>
#include <QFile>


QHash<QString,QWidget*> *WidgetShot::mWidgetList = 0;


void WidgetShot::addWidget( const QString &widgetName, QWidget *widget )
{
   if( !mWidgetList )
   {
      mWidgetList = new QHash<QString,QWidget*>();
   }
   mWidgetList->insert( widgetName, widget );
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
