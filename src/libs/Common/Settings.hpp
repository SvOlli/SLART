/**
 * src/libs/Common/Settings.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef SETTINGS_HPP
#define SETTINGS_HPP SETTINGS_HPP

/* base class */
#include <QObject>

/* system headers */

/* Qt headers */
#include <QCoreApplication>
#include <QDir>
#include <QPoint>
#include <QSize>

/* local library headers */
#include "MySettings.hpp"

/* local headers */

/* forward declaration of Qt classes */

/* forward declaration of local classes */

/*!
  \addtogroup Common
  @{
  */

/*!
 \brief abstraction layer

*/
class Settings : public QObject
{
   Q_OBJECT

public:

#include "SettingsGenerated.hpp"

private:
   /*!
    \brief get MySettings pointer according to application name

    \param applicationName
   */
   static MySettings *get( const QString &applicationName = QCoreApplication::applicationName() );

   /*!
    \brief get MySettings pointer according to application name

    \param applicationName
   */
   static void remove( const QString &key,
                       const QString &applicationName = QString() );

   /*!
    \brief constructor

    \param parent parent QObject
   */
   Settings( QObject *parent = 0 );

   /*!
    \brief destructor

   */
   virtual ~Settings();

   QMap<QString,MySettings*>  mSettings; /*!< map for storing settings pointers */
   static Settings            *cpSettings; /*!< handle for static functions */
};

/*! @} */

#endif
