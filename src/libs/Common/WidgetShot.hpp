/**
 * src/libs/Common/WidgetShot.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef WIDGETSHOT_HPP
#define WIDGETSHOT_HPP WIDGETSHOT_HPP

#include <QString>
#include <QWidget>
#include <QHash>

class WidgetShot
{
public:
   static void addWidget( const QString &widgetName, QWidget *widget );
   static void shootWidget( const QString &widgetName, const QString &fileName );
   
private:
   WidgetShot();
   WidgetShot( const WidgetShot &other );
   WidgetShot &operator=( const WidgetShot &other );
   
   static QHash<QString,QWidget*> *mWidgetList;
};

#endif
