/*
 * src/libs/Common/MySettings.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef MYSETTINGS_HPP
#define MYSETTINGS_HPP MYSETTINGS_HPP

/* base class */
#include <QSettings>

/* system headers */

/* Qt headers */

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */
class QMainWindow;

/* forward declaration of local classes */


/*!
 \addtogroup Common

 @{
*/

/*!
 \brief adopted version of QSettings

 \todo this class is depricated, adopt all code to "Settings" and remove it
*/
class MySettings : public QSettings
{
   Q_OBJECT

public:
   /*!
    \brief constructor

    \param parent parent object
   */
   MySettings( QObject *parent = 0 );

   /*!
    \brief constructor

    \param application application name
    \param parent parent object
   */
   MySettings( const QString &application, QObject *parent = 0 );

   /*!
    \brief destructor

   */
   virtual ~MySettings();

   /*!
    \brief get the filename of the style sheet to load

   */
   QString styleSheetFile();

private:
   Q_DISABLE_COPY( MySettings )
};

#endif
