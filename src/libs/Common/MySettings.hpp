/**
 * src/libs/Common/MySettings.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef MYSETTINGS_HPP
#define MYSETTINGS_HPP MYSETTINGS_HPP

#include <QSettings>

class QMainWindow;

/*!
  \addtogroup Common

  @{
  */

/*!
 \brief

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
    \brief save main window settings

    \param mainWindow
   */
   void saveMainWindow( QMainWindow *mainWindow );

   /*!
    \brief restore main window settings

    \param mainWindow
   */
   void setMainWindow( QMainWindow *mainWindow );

   /*!
    \brief get the filename of the style sheet to load

   */
   QString styleSheetFile();

private:
   Q_DISABLE_COPY( MySettings )
};

#endif
