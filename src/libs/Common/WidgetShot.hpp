/*
 * src/libs/Common/WidgetShot.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef WIDGETSHOT_HPP
#define WIDGETSHOT_HPP WIDGETSHOT_HPP

/* base class */
#include <QObject>

/* system headers */

/* Qt headers */
#include <QHash>
#include <QPointer>
#include <QString>
#include <QWidget>

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */

/* forward declaration of local classes */


/*!
  \addtogroup Common

  @{
*/

/*!
 \brief static methods to enable "screenshots" of application windows / widgets

*/
class WidgetShot
{
public:
   /*!
    \brief

    \param widgetName
    \param widget
   */
   static void addWidget( const QString &widgetName, QWidget *widget );
   /*!
    \brief

    \param widgetName
    \param fileName
   */
   static void shootWidget( const QString &widgetName, const QString &fileName );

private:

   /*!
    \brief private construtor, class only uses static methods

    \param
   */
   WidgetShot();

   Q_DISABLE_COPY( WidgetShot )

   static QHash< QString, QPointer<QWidget> > *cpWidgetList; /*!< \brief \todo complete documentation */
};

/*! @} */

#endif
