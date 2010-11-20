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


class MySettings : public QSettings
{
public:
   MySettings();
   MySettings( const QString &application );

   /* save main window settings */
   void saveMainWindow( QMainWindow *mainWindow );
   /* restore main window settings */
   void setMainWindow( QMainWindow *mainWindow );

   /* get the filename of the style sheet to load */
   QString styleSheetFile();
};

#endif
