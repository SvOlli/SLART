/*
 * src/tests/TestAppSatellite/MainWindow.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP MAINWINDOW_HPP

/* base class */
#include <QMainWindow>

/* system headers */

/* Qt headers */

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */
class QApplication;
class QDir;

/* forward declaration of local classes */


class MainWindow : public QMainWindow
{
   Q_OBJECT

public:
   MainWindow( QWidget *parent = 0, Qt::WindowFlags flags = 0 );

private:
   MainWindow( const MainWindow &that );
   MainWindow &operator=( const MainWindow &that );
};

#endif
